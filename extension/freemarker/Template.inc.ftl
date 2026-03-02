<#macro template_definition templateParameterList>
    template <<#t>
    <#list templateParameterList as templateParameter>
        typename ${templateParameter.name}<#sep>, </#sep><#t>
    </#list>
    <#lt>>
</#macro>

<#macro template_parameter_aliases templateParameterList>
    <#list templateParameterList>
        <#items as param>
    using ${param.typeAliasName} = ${param.name};
        </#items>

    </#list>
</#macro>