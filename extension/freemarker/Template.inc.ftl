<#macro template_definition templateParametes>
    template <<#t>
    <#list templateParameters as templateParameter>
        typename ${templateParameter}<#sep>, </#sep><#t>
    </#list>
    <#lt>>
</#macro>
