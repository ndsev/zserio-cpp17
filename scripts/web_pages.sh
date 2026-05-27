#!/bin/bash

SCRIPT_DIR=`dirname $0`
source "${SCRIPT_DIR}/common_tools.sh"

# Set and check global variables.
set_web_pages_global_variables()
{
    # GIT to use, defaults to "git" if not set
    GIT="${GIT:-git}"
    if [ ! -f "`which "${GIT}"`" ] ; then
        stderr_echo "Cannot find git! Set GIT environment variable."
        return 1
    fi
}

# Print help message.
print_help()
{
    cat << EOF
Description:
    Update web pages after new Zserio C++17 extension release.

Usage:
    $0 [-h] [-e] [-o <dir>]

Arguments:
    -h, --help               Show this help.
    -e, --help-env           Show help for enviroment variables.
    -o <dir>, --output-directory <dir>
                             Output directory where build and distr will be located.

Examples:
    $0

EOF
}

# Print help on the environment variables.
print_help_env()
{
    cat << EOF
Uses the following environment variables:
    GIT      Git executable to use. Default is "git".

    Either set these directly, or create 'scripts/build-env.sh' that sets these.
    It's sourced automatically if it exists.

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
    exit_if_argc_lt $# 1
    local PARAM_OUT_DIR_OUT="$1"; shift

    local ARG="$1"
    while [ $# -ne 0 ] ; do
        case "${ARG}" in
            "-h" | "--help")
                return 2
                ;;

            "-e" | "--help-env")
                return 3
                ;;

            "-o" | "--output-directory")
                if [ $# -eq 1 ] ; then
                    stderr_echo "Missing output directory!"
                    echo
                    return 1
                fi
                eval ${PARAM_OUT_DIR_OUT}="$2"
                shift 2
                ;;
        esac
        ARG="$1"
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
    local CLANG_LINES_COVERAGE=`cat "${CLANG_COVERAGE_DIR}"/coverage_report.txt | grep TOTAL | \
            tr -s ' ' | cut -d' ' -f 10`
    create_github_badge_json "${CLANG_COVERAGE_DIR}"/coverage_github_badge.json \
            "C++ clang runtime ${ZSERIO_CPP17_VERSION} coverage" "${CLANG_LINES_COVERAGE}"
    if [ $? -ne 0 ] ; then
        return 1
    fi

    return 0
}

# Create JSON configuration file for GitHub badge
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

# Patch new runtime documentations - add cross references between runtime versions
patch_new_runtime_doc()
{
    exit_if_argc_ne $# 3
    local ZSERIO_CPP17_DOC_RUNTIME_DIR="$1"; shift
    local ZSERIO_PATCH_DOC_DIR="$1"; shift
    local ZSERIO_CPP17_VERSION="$1"; shift

    local ZSERIO_CPP17_VERSION_SELECT="\n\
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
    local HTML_FILES=($(grep "Built for Zserio" "${ZSERIO_PATCH_DOC_DIR}" -R -l ${GREP_INCLUDE[@]}))
    for HTML_FILE  in "${HTML_FILES[@]}" ; do
        sed -i 's/\(Built for Zserio\)\s*[a-zA-Z0-9.-]*/\1'"${ZSERIO_CPP17_VERSION_SELECT}"'/' "${HTML_FILE}"
        if [ $? -ne 0 ] ; then
            stderr_echo "Failed to apply zserio-version-select!"
            return 1
        fi
    done

    return 0
}

main()
{
    local ZSERIO_CPP17_PROJECT_ROOT="${SCRIPT_DIR}/.."

    # parse command line arguments
    local PARAM_OUT_DIR="${ZSERIO_CPP17_PROJECT_ROOT}"
    parse_arguments PARAM_OUT_DIR "$@"
    local PARSE_RESULT=$?
    if [ ${PARSE_RESULT} -eq 2 ] ; then
        print_help
        return 0
    elif [ ${PARSE_RESULT} -eq 3 ] ; then
        print_help_env
        return 0
    elif [ ${PARSE_RESULT} -ne 0 ] ; then
        return 1
    fi

    # get the output directory (the absolute path is necessary)
    convert_to_absolute_path "${PARAM_OUT_DIR}" PARAM_OUT_DIR

    set_web_pages_global_variables

    # get latest Zserio C++17 version
    local ZSERIO_CPP17_VERSION
    get_latest_zserio_cpp17_version ZSERIO_CPP17_VERSION
    if [ $? -ne 0 ] ; then
        return 1
    fi

    local ZSERIO_BUILD_DIR="${PARAM_OUT_DIR}/build"
    local ZSERIO_DISTR_DIR="${PARAM_OUT_DIR}/distr"
    local GIT_MESSAGE="Add generated runtime documentation v${ZSERIO_CPP17_VERSION}"

    echo "Rebasing Zserio C++17 Extension Web Pages branch onto the master tag."

    "${GIT}" checkout web-pages 2>&1 > /dev/null
    if [ $? -ne 0 ] ; then
        "${GIT}" checkout -b web-pages
        local GIT_RESULT=$?
        if [ $GIT_RESULT -ne 0 ] ; then
            stderr_echo "Git failed with return code ${GIT_RESULT}!"
            return 1
        fi
    fi
    "${GIT}" fetch --all
    local GIT_RESULT=$?
    if [ ${GIT_RESULT} -ne 0 ] ; then
        stderr_echo "Git failed with return code ${GIT_RESULT}!"
        return 1
    fi
    "${GIT}" rebase master
    local GIT_RESULT=$?
    if [ ${GIT_RESULT} -ne 0 ] ; then
        stderr_echo "Git failed with return code ${GIT_RESULT}!"
        return 1
    fi
    echo

    local DEST_RUNTIME_DIR="${ZSERIO_CPP17_PROJECT_ROOT}/doc/runtime"

    echo -ne "Removing Zserio C++17 extension runtime libraries version ${ZSERIO_CPP17_VERSION}..."
    local DEST_VERSION_DIR="${DEST_RUNTIME_DIR}/${ZSERIO_CPP17_VERSION}"
    rm -rf "${DEST_VERSION_DIR}"
    echo "Done"

    echo -ne "Copying Zserio C++17 extension runtime library version ${ZSERIO_CPP17_VERSION}..."
    mkdir -p "${DEST_VERSION_DIR}"
    cp -r ${ZSERIO_DISTR_DIR}/runtime_lib/zserio_doc/* ${DEST_VERSION_DIR}
    if [ $? -ne 0 ] ; then
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

    echo -ne "Removing Zserio C++17 extension runtime libraries latest version..."
    local DEST_LATEST_DIR="${DEST_RUNTIME_DIR}/latest"
    rm -rf "${DEST_LATEST_DIR}"
    echo "Done"

    echo -ne "Copying Zserio C++17 extension runtime library latest version..."
    mkdir -p "${DEST_LATEST_DIR}"
    cp -r ${ZSERIO_DISTR_DIR}/runtime_lib/zserio_doc/* ${DEST_LATEST_DIR}
    if [ $? -ne 0 ] ; then
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
    echo "Committing changes to Zserio C++17 extension Web Pages branch."
    "${GIT}" -C "${ZSERIO_CPP17_PROJECT_ROOT}" add -A
    "${GIT}" -C "${ZSERIO_CPP17_PROJECT_ROOT}" commit -a -m "${GIT_MESSAGE}"
    local GIT_RESULT=$?
    if [ ${GIT_RESULT} -ne 0 ] ; then
        stderr_echo "Git failed with return code ${GIT_RESULT}!"
        return 1
    fi

    echo $'\e[1;33m'"Don't forget to check the 'web-pages' branch!"$'\e[0m'
    read -n 1 -s -r -p "Press any key to PUSH the 'web-pages' branch..."
    echo

    "${GIT}" -C "${ZSERIO_CPP17_PROJECT_ROOT}" push --force --set-upstream origin web-pages
    local GIT_RESULT=$?
    if [ ${GIT_RESULT} -ne 0 ] ; then
        stderr_echo "Git failed with return code ${GIT_RESULT}!"
        return 1
    fi

    "${GIT}" -C "${ZSERIO_CPP17_PROJECT_ROOT}" checkout master
    local GIT_RESULT=$?
    if [ ${GIT_RESULT} -ne 0 ] ; then
        stderr_echo "Git failed with return code ${GIT_RESULT}!"
        return 1
    fi
    echo

    return 0
}

main "$@"
