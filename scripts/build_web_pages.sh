#!/bin/bash

SCRIPT_DIR=`dirname $0`
source "${SCRIPT_DIR}/common_tools.sh"

# Set and check global variables.
set_build_web_pages_global_variables()
{
    # SED to use, defaults to "sed" if not set; GNU version is required
    # (BSD sed silently does something else with the options used here)
    SED="${SED:-sed}"
    if ! "${SED}" --version 2>/dev/null | grep -q "GNU" ; then
        stderr_echo "GNU sed is required, '${SED}' is not GNU sed! Set SED environment variable" \
                "(e.g. SED=gsed on macOS)."
        return 1
    fi

    # UNZIP to use, defaults to "unzip" if not set
    UNZIP="${UNZIP:-unzip}"
    if [ ! -f "`which "${UNZIP}"`" ] ; then
        stderr_echo "Cannot find unzip! Set UNZIP environment variable."
        return 1
    fi

    return 0
}

# Print help on the environment variables used for this script.
print_build_web_pages_help_env()
{
    cat << EOF
Uses the following environment variables for building of Zserio C++17 extension Web Pages:
    SED      GNU sed executable to use. Default is "sed" (on macOS e.g. "gsed").
    UNZIP    Unzip executable to use. Default is "unzip".

    Either set these directly, or create 'scripts/build-env.sh' that sets these.
    It's sourced automatically if it exists.

EOF
}

# Add a new version <option> to the version select of all already published runtime documentations.
patch_old_runtime_doc()
{
    exit_if_argc_ne $# 2
    local ZSERIO_DOC_DIR="$1"; shift
    local ZSERIO_CPP17_VERSION="$1"; shift

    local PATTERN="<select id=\"zserio-version-select\""

    local HTML_FILES=($(grep "${PATTERN}" "${ZSERIO_DOC_DIR}" -R -l))
    for HTML_FILE  in "${HTML_FILES[@]}" ; do
        "${SED}" -i '/'"${PATTERN}"'/a <option value="'"${ZSERIO_CPP17_VERSION}"'">'"${ZSERIO_CPP17_VERSION}"'</option>' ${HTML_FILE}
        if [ $? -ne 0 ] ; then
            stderr_echo "Failed to append the new version <option>!"
            return 1
        fi
    done

    return 0
}

# Replace the version in the new runtime documentation by the select of all published versions.
patch_new_runtime_doc()
{
    exit_if_argc_ne $# 3
    local ZSERIO_CPP17_DOC_RUNTIME_DIR="$1"; shift
    local ZSERIO_PATCH_DOC_DIR="$1"; shift
    local ZSERIO_CPP17_VERSION="$1"; shift

    local ZSERIO_CPP17_VERSION_SELECT="<\/div>\n\
<div id=\"projectbrief\">C++17 Extension version \
<select id=\"zserio-version-select\" style=\"font-size: 100%; margin-bottom: 1px; padding: 2px;\"\
 onChange=\"(function(value){ var url = top.document.URL.split('\/'); url[url.length-2] = \`\${value}\`;\
 top.location.href=url.join('\/'); \
})(value)\">\n\
<option value=\"${ZSERIO_CPP17_VERSION}\" selected>${ZSERIO_CPP17_VERSION}<\/option>\n\
"
    local OLD_VERSIONS=($(ls -1 "${ZSERIO_CPP17_DOC_RUNTIME_DIR}" | sort -rV))
    for OLD_VERSION in ${OLD_VERSIONS[@]}; do
        if [ ${OLD_VERSION} != ${ZSERIO_CPP17_VERSION} -a ${OLD_VERSION} != "latest" ] ; then
            ZSERIO_CPP17_VERSION_SELECT+="<option value=\"${OLD_VERSION}\">${OLD_VERSION}<\/option>\n"
        fi
    done
    ZSERIO_CPP17_VERSION_SELECT+="<\/select>\n"

    local GREP_INCLUDE=(--include "index.html" --include "zserio.html" --include "overview-summary.html")
    local HTML_FILES=($(grep "Zserio C++17 runtime library" "${ZSERIO_PATCH_DOC_DIR}" -R -l ${GREP_INCLUDE[@]}))
    for HTML_FILE  in "${HTML_FILES[@]}" ; do
        "${SED}" -i 's/<span id="projectnumber">.*<\/span>/'"${ZSERIO_CPP17_VERSION_SELECT}"'/' "${HTML_FILE}"
        if [ $? -ne 0 ] ; then
            stderr_echo "Failed to apply zserio-version-select!"
            return 1
        fi
    done

    return 0
}

# Create JSON configuration files for all GitHub badges
create_github_badge_jsons()
{
    exit_if_argc_ne $# 2
    local DEST_RUNTIME_DIR="$1"; shift
    local ZSERIO_CPP17_VERSION="$1"; shift

    local CLANG_ROOT="${DEST_RUNTIME_DIR}/${ZSERIO_CPP17_VERSION}/coverage/"
    local CLANG_COVERAGE_DIR=`find ${CLANG_ROOT} -maxdepth 1 -name "clang*" | head -1`
    if [ -z "${CLANG_COVERAGE_DIR}" ] ; then
        stderr_echo "Cannot find clang coverage report in ${CLANG_ROOT}!"
        return 1
    fi
    local CLANG_LINES_COVERAGE=`cat "${CLANG_COVERAGE_DIR}"/coverage_report.txt | grep TOTAL | \
            tr -s ' ' | cut -d' ' -f 10`
    create_github_badge_json "${CLANG_COVERAGE_DIR}"/coverage_github_badge.json \
            "C++ clang runtime ${ZSERIO_CPP17_VERSION} coverage" "${CLANG_LINES_COVERAGE}"
    if [ $? -ne 0 ] ; then
        return 1
    fi

    return 0
}

# Create JSON configuration file for one GitHub badge
create_github_badge_json()
{
    exit_if_argc_ne $# 3
    local BADGE_JSON_FILE="$1"; shift
    local BADGE_LABEL="$1"; shift
    local BADGE_MESSAGE="$1"; shift

    cat > "${BADGE_JSON_FILE}" << EOF
{
    "schemaVersion": 1,
    "label": "${BADGE_LABEL}",
    "message": "${BADGE_MESSAGE}",
    "color": "green"
}
EOF

    if [ $? -ne 0 ] ; then
        stderr_echo "Failed to create badge json file!"
        echo
        return 1
    fi

    return 0
}

# Add the runtime documentation of one new Zserio C++17 extension version to the runtime documentation directory.
add_runtime_doc()
{
    exit_if_argc_ne $# 4
    local DEST_RUNTIME_DIR="$1"; shift
    local WORK_DIR="$1"; shift
    local RUNTIME_LIB_ZIP="$1"; shift
    local ZSERIO_CPP17_VERSION="$1"; shift

    echo "Adding Zserio C++17 extension runtime library documentation version ${ZSERIO_CPP17_VERSION}."
    local UNZIP_DIR="${WORK_DIR}/${ZSERIO_CPP17_VERSION}"
    rm -rf "${UNZIP_DIR}"
    mkdir -p "${UNZIP_DIR}"

    echo -ne "Unzipping Zserio C++17 extension runtime library..."
    "${UNZIP}" -q "${RUNTIME_LIB_ZIP}" -d "${UNZIP_DIR}"
    if [ $? -ne 0 ] ; then
        stderr_echo "Cannot unzip Zserio C++17 extension runtime library to ${UNZIP_DIR}!"
        return 1
    fi
    if [ ! -d "${UNZIP_DIR}/zserio_doc" ] ; then
        stderr_echo "Zserio C++17 extension runtime library zip does not contain zserio_doc!"
        return 1
    fi
    echo "Done"

    echo -ne "Removing Zserio C++17 extension runtime library latest version..."
    local DEST_LATEST_DIR="${DEST_RUNTIME_DIR}/latest"
    rm -rf "${DEST_LATEST_DIR}"
    echo "Done"

    echo -ne "Adding cross references between runtime libraries versions to old documentations..."
    patch_old_runtime_doc "${DEST_RUNTIME_DIR}" "${ZSERIO_CPP17_VERSION}"
    if [ $? -ne 0 ] ; then
        return 1
    fi
    echo "Done"

    echo -ne "Copying Zserio C++17 extension runtime library version ${ZSERIO_CPP17_VERSION}..."
    local DEST_VERSION_DIR="${DEST_RUNTIME_DIR}/${ZSERIO_CPP17_VERSION}"
    mkdir -p "${DEST_VERSION_DIR}"
    cp -r "${UNZIP_DIR}"/zserio_doc/* "${DEST_VERSION_DIR}"
    if [ $? -ne 0 ] ; then
        stderr_echo "Cannot copy Zserio C++17 extension runtime library documentation!"
        return 1
    fi
    echo "Done"

    echo -ne "Adding cross references between runtime libraries versions in version ${ZSERIO_CPP17_VERSION}..."
    patch_new_runtime_doc "${DEST_RUNTIME_DIR}" "${DEST_VERSION_DIR}" "${ZSERIO_CPP17_VERSION}"
    if [ $? -ne 0 ] ; then
        return 1
    fi
    echo "Done"

    echo -ne "Creating Zserio C++17 extension runtime library version ${ZSERIO_CPP17_VERSION} GitHub badges..."
    create_github_badge_jsons "${DEST_RUNTIME_DIR}" "${ZSERIO_CPP17_VERSION}"
    if [ $? -ne 0 ] ; then
        return 1
    fi
    echo "Done"

    echo -ne "Copying Zserio C++17 extension runtime library latest version..."
    mkdir -p "${DEST_LATEST_DIR}"
    cp -r "${UNZIP_DIR}"/zserio_doc/* "${DEST_LATEST_DIR}"
    if [ $? -ne 0 ] ; then
        stderr_echo "Cannot copy Zserio C++17 extension runtime library documentation!"
        return 1
    fi
    echo "Done"

    echo -ne "Adding cross references between runtime libraries versions in latest version..."
    patch_new_runtime_doc "${DEST_RUNTIME_DIR}" "${DEST_LATEST_DIR}" "${ZSERIO_CPP17_VERSION}"
    if [ $? -ne 0 ] ; then
        return 1
    fi
    echo "Done"

    echo -ne "Creating Zserio C++17 extension runtime library latest version GitHub badges..."
    create_github_badge_jsons "${DEST_RUNTIME_DIR}" "latest"
    if [ $? -ne 0 ] ; then
        return 1
    fi
    echo "Done"
    echo

    return 0
}

# Assemble the Zserio C++17 extension Web Pages sources which are built by Jekyll.
build_web_pages()
{
    exit_if_argc_lt $# 4
    local ZSERIO_CPP17_PROJECT_ROOT="$1"; shift
    local SOURCE_DIR="$1"; shift
    local ARCHIVE_DIR="$1"; shift
    local SITE_DIR="$1"; shift
    local RUNTIME_LIB_ZIPS=("$@")

    echo "Copying Zserio C++17 extension sources from ${SOURCE_DIR}."
    rm -rf "${SITE_DIR}"
    mkdir -p "${SITE_DIR}"
    tar -C "${SOURCE_DIR}" --exclude=.git -cf - . | tar -C "${SITE_DIR}" -xf -
    if [ ${PIPESTATUS[0]} -ne 0 -o ${PIPESTATUS[1]} -ne 0 ] ; then
        stderr_echo "Cannot copy Zserio C++17 extension sources to ${SITE_DIR}!"
        return 1
    fi
    cp "${ZSERIO_CPP17_PROJECT_ROOT}/.github/pages/_config.yml" "${SITE_DIR}"
    if [ $? -ne 0 ] ; then
        stderr_echo "Cannot copy Jekyll configuration to ${SITE_DIR}!"
        return 1
    fi

    echo "Copying Zserio C++17 extension runtime library documentation archive from ${ARCHIVE_DIR}."
    local DEST_RUNTIME_DIR="${SITE_DIR}/doc/runtime"
    if [ -e "${DEST_RUNTIME_DIR}" ] ; then
        stderr_echo "Zserio C++17 extension sources must not contain ${DEST_RUNTIME_DIR#${SITE_DIR}/}!"
        return 1
    fi
    mkdir -p "${DEST_RUNTIME_DIR}"
    cp -r "${ARCHIVE_DIR}"/* "${DEST_RUNTIME_DIR}"
    if [ $? -ne 0 ] ; then
        stderr_echo "Cannot copy runtime library documentation archive to ${DEST_RUNTIME_DIR}!"
        return 1
    fi
    echo

    local WORK_DIR="${SITE_DIR}.work"
    rm -rf "${WORK_DIR}"
    local RUNTIME_LIB_ZIP
    for RUNTIME_LIB_ZIP in "${RUNTIME_LIB_ZIPS[@]}" ; do
        local ZIP_NAME="${RUNTIME_LIB_ZIP##*/}"
        if [[ ! "${ZIP_NAME}" =~ ^zserio-cpp17-([0-9]+\.[0-9]+\.[0-9]+)-runtime-lib\.zip$ ]] ; then
            stderr_echo "Invalid runtime library zip name '${ZIP_NAME}'!"
            return 1
        fi
        local ZSERIO_CPP17_VERSION="${BASH_REMATCH[1]}"
        local NEWEST_VERSION=`ls -1 "${DEST_RUNTIME_DIR}" | grep -v "^latest$" | sort -V | tail -n 1`
        if [[ "${NEWEST_VERSION}" == "${ZSERIO_CPP17_VERSION}" ||
              "`printf "%s\n%s\n" "${NEWEST_VERSION}" "${ZSERIO_CPP17_VERSION}" | sort -V | tail -n 1`" != \
                    "${ZSERIO_CPP17_VERSION}" ]] ; then
            stderr_echo "Version ${ZSERIO_CPP17_VERSION} is not newer than the published version ${NEWEST_VERSION}!"
            return 1
        fi

        add_runtime_doc "${DEST_RUNTIME_DIR}" "${WORK_DIR}" "${RUNTIME_LIB_ZIP}" "${ZSERIO_CPP17_VERSION}"
        if [ $? -ne 0 ] ; then
            return 1
        fi
    done
    rm -rf "${WORK_DIR}"

    echo "Zserio C++17 extension Web Pages sources are in ${SITE_DIR}."

    return 0
}

# Print help message.
print_help()
{
    cat << EOF
Description:
    Assemble the sources of Zserio C++17 extension Web Pages which are then built by Jekyll.

    The site consists of the Zserio C++17 extension sources, the Jekyll configuration
    .github/pages/_config.yml, the archived runtime library documentation of all already published
    versions and the runtime library documentation of the given new versions.

Usage:
    $0 [-h] [-e] -s <dir> -a <dir> -o <dir> [runtime_lib_zip...]

Arguments:
    -h, --help       Show this help.
    -e, --help-env   Show help for enviroment variables.
    -s <dir>, --source-directory <dir>
                     Zserio C++17 extension sources to publish, e.g. a checkout of the release tag with
                     submodules. Required.
    -a <dir>, --archive-directory <dir>
                     Archived runtime library documentation (doc/runtime of the published site). Required.
    -o <dir>, --output-directory <dir>
                     Directory where the site sources are assembled. It is removed first. Required.

    runtime_lib_zip  Zserio C++17 extension runtime library zip zserio-cpp17-<version>-runtime-lib.zip
                     of a version newer than all archived versions. Several zips are added in the given order.

Examples:
    $0 -s . -a ../web-pages-archive/doc/runtime -o build/web_pages/site \\
            zserio-cpp17-1.3.0-runtime-lib.zip

EOF
}

# Parse all command line arguments.
#
# Return codes:
# -------------
# 0 - Success. Arguments have been successfully parsed.
# 1 - Failure. Some arguments are wrong or missing.
# 2 - Help switch is present. Arguments after help switch have not been checked.
# 3 - Environment help switch is present. Arguments after help switch have not been checked.
parse_arguments()
{
    exit_if_argc_lt $# 4
    local PARAM_SOURCE_DIR_OUT="$1"; shift
    local PARAM_ARCHIVE_DIR_OUT="$1"; shift
    local PARAM_SITE_DIR_OUT="$1"; shift
    local PARAM_RUNTIME_LIB_ZIPS_OUT="$1"; shift

    eval ${PARAM_RUNTIME_LIB_ZIPS_OUT}="()"
    local NUM_ZIPS=0
    while [ $# -ne 0 ] ; do
        local ARG="$1"
        case "${ARG}" in
            "-h" | "--help")
                return 2
                ;;

            "-e" | "--help-env")
                return 3
                ;;

            "-s" | "--source-directory" | "-a" | "--archive-directory" | "-o" | "--output-directory")
                if [ $# -eq 1 ] ; then
                    stderr_echo "Missing directory for '${ARG}'!"
                    echo
                    return 1
                fi
                case "${ARG}" in
                    "-s" | "--source-directory") eval ${PARAM_SOURCE_DIR_OUT}="\"$2\"" ;;
                    "-a" | "--archive-directory") eval ${PARAM_ARCHIVE_DIR_OUT}="\"$2\"" ;;
                    *) eval ${PARAM_SITE_DIR_OUT}="\"$2\"" ;;
                esac
                shift 2
                ;;

            "-"*)
                stderr_echo "Invalid switch '${ARG}'!"
                echo
                return 1
                ;;

            *)
                eval ${PARAM_RUNTIME_LIB_ZIPS_OUT}[${NUM_ZIPS}]="\"${ARG}\""
                NUM_ZIPS=$((NUM_ZIPS + 1))
                shift
                ;;
        esac
    done

    if [[ -z "${!PARAM_SOURCE_DIR_OUT}" || -z "${!PARAM_ARCHIVE_DIR_OUT}" || -z "${!PARAM_SITE_DIR_OUT}" ]] ; then
        stderr_echo "Source, archive and output directories are required!"
        echo
        return 1
    fi

    return 0
}

# Main entry of the script to build Zserio C++17 extension Web Pages.
main()
{
    # get the project root
    local ZSERIO_CPP17_PROJECT_ROOT="${SCRIPT_DIR}/.."

    # parse command line arguments
    local PARAM_SOURCE_DIR=""
    local PARAM_ARCHIVE_DIR=""
    local PARAM_SITE_DIR=""
    local PARAM_RUNTIME_LIB_ZIPS
    parse_arguments PARAM_SOURCE_DIR PARAM_ARCHIVE_DIR PARAM_SITE_DIR PARAM_RUNTIME_LIB_ZIPS "$@"
    local PARSE_RESULT=$?
    if [ ${PARSE_RESULT} -eq 2 ] ; then
        print_help
        return 0
    elif [ ${PARSE_RESULT} -eq 3 ] ; then
        print_build_web_pages_help_env
        return 0
    elif [ ${PARSE_RESULT} -ne 0 ] ; then
        print_help
        return 1
    fi

    # set global variables
    set_build_web_pages_global_variables
    if [ $? -ne 0 ] ; then
        return 1
    fi

    # absolute paths are necessary, the zips are checked before anything is removed
    convert_to_absolute_path "${ZSERIO_CPP17_PROJECT_ROOT}" ZSERIO_CPP17_PROJECT_ROOT
    local DIR_PARAM
    for DIR_PARAM in PARAM_SOURCE_DIR PARAM_ARCHIVE_DIR ; do
        if [ ! -d "${!DIR_PARAM}" ] ; then
            stderr_echo "Directory '${!DIR_PARAM}' does not exist!"
            return 1
        fi
        convert_to_absolute_path "${!DIR_PARAM}" ${DIR_PARAM}
    done
    mkdir -p "${PARAM_SITE_DIR}"
    convert_to_absolute_path "${PARAM_SITE_DIR}" PARAM_SITE_DIR
    local I
    for I in "${!PARAM_RUNTIME_LIB_ZIPS[@]}" ; do
        if [ ! -f "${PARAM_RUNTIME_LIB_ZIPS[${I}]}" ] ; then
            stderr_echo "Runtime library zip '${PARAM_RUNTIME_LIB_ZIPS[${I}]}' does not exist!"
            return 1
        fi
        convert_to_absolute_path "${PARAM_RUNTIME_LIB_ZIPS[${I}]}" PARAM_RUNTIME_LIB_ZIPS[${I}]
    done

    build_web_pages "${ZSERIO_CPP17_PROJECT_ROOT}" "${PARAM_SOURCE_DIR}" "${PARAM_ARCHIVE_DIR}" \
            "${PARAM_SITE_DIR}" "${PARAM_RUNTIME_LIB_ZIPS[@]}"
}

main "$@"
