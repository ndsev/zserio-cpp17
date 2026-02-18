<#macro template_definition templateParameterList>
    template <<#t>
    <#list templateParameterList as templateParameter>
        typename ${templateParameter.name}<#sep>, </#sep><#t>
    </#list>
    <#lt>>
</#macro>
