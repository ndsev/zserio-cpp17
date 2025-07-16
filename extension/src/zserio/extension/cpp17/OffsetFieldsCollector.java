package zserio.extension.cpp17;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import zserio.ast.ArrayInstantiation;
import zserio.ast.ChoiceType;
import zserio.ast.CompoundType;
import zserio.ast.Field;
import zserio.ast.Root;
import zserio.ast.StructureType;
import zserio.ast.TypeInstantiation;
import zserio.ast.UnionType;
import zserio.ast.ZserioAstWalker;
import zserio.ast.ZserioTemplatableType;
import zserio.ast.ZserioType;

/**
 * Implements collectors which traverse source tree and detects which types are used within a packed array.
 *
 * Intended to be used by generators to optimize the generated code such that types which are not used within
 * a packed arrays don't need to generate methods needed only for packing.
 */
public class OffsetFieldsCollector extends ZserioAstWalker
{
    @Override
    public void visitRoot(Root root)
    {
        super.visitRoot(root);
        firstRun = false;
        super.visitRoot(root);
    }

    @Override
    public void visitStructureType(StructureType structureType)
    {
        visitCompound(structureType);
    }

    @Override
    public void visitChoiceType(ChoiceType choiceType)
    {
        visitCompound(choiceType);
    }

    @Override
    public void visitUnionType(UnionType unionType)
    {
        visitCompound(unionType);
    }

    /**
     * Checks if the given field is used as an offset.
     *
     * @param field Field to check.
     *
     * @return True when the given field is used as an offset, false otherwise.
     */
    public boolean isUsedAsOffset(Field field)
    {
        return offsetFields.contains(field);
    }

    /**
     * Checks if the given compound type contains an offset within the subtree.
     *
     * @param compoundType Compound type to check.
     *
     * @return True when the given compound type contains an offset, false otherwise.
     */
    public boolean containsOffset(CompoundType compoundType)
    {
        return compoundsWithOffset.contains(compoundType);
    }

    private void visitCompound(CompoundType compoundType)
    {
        if (!compoundType.getTemplateParameters().isEmpty())
        {
            // visit instantiations
            for (ZserioTemplatableType instantiation : compoundType.getInstantiations())
                instantiation.accept(this);
        }
        else
        {
            if (firstRun)
            {
                compoundTypeStack.add(compoundType);

                for (Field field : compoundType.getFields()) // it's enough to traverse fields
                {
                    if (field.getOffsetExpr() != null)
                    {
                        final Map<Field, CompoundType> referencedFieldObjects =
                                field.getOffsetExpr().getReferencedFieldsWithOwner();
                        if (!referencedFieldObjects.isEmpty())
                        {
                            final Map.Entry<Field, CompoundType> referencedFieldEntry =
                                    referencedFieldObjects.entrySet().iterator().next();
                            addOffsetField(referencedFieldEntry.getKey(), referencedFieldEntry.getValue());
                        }

                        addCompoundsWithOffset(compoundTypeStack);
                    }

                    TypeInstantiation typeInstantiation = field.getTypeInstantiation();
                    if (typeInstantiation instanceof ArrayInstantiation)
                    {
                        final ArrayInstantiation arrayInstantiation = (ArrayInstantiation)typeInstantiation;
                        typeInstantiation = arrayInstantiation.getElementTypeInstantiation();
                    }
                    final ZserioType baseType = typeInstantiation.getBaseType();
                    if (baseType instanceof CompoundType && !compoundType.equals(baseType))
                    {
                        baseType.accept(this);
                    }
                }

                compoundTypeStack.remove(compoundTypeStack.size() - 1);
            }
            else
            {
                for (Field field : compoundType.getFields())
                {
                    TypeInstantiation typeInstantiation = field.getTypeInstantiation();
                    if (typeInstantiation instanceof ArrayInstantiation)
                    {
                        final ArrayInstantiation arrayInstantiation = (ArrayInstantiation)typeInstantiation;
                        typeInstantiation = arrayInstantiation.getElementTypeInstantiation();
                    }
                    final ZserioType baseType = typeInstantiation.getBaseType();
                    if (baseType instanceof ChoiceType || baseType instanceof UnionType)
                    {
                        if (!compoundType.equals(baseType))
                        {
                            baseType.accept(this);
                        }

                        final CompoundType fieldCompoundType = (CompoundType)baseType;
                        for (Field childField : fieldCompoundType.getFields())
                        {
                            if (offsetFields.contains(childField))
                            {
                                addOffsetField(field, compoundType);
                            }
                        }
                    }
                }
            }
        }
    }

    private void addOffsetField(Field offsetField, CompoundType offsetFieldOwner)
    {
        offsetFields.add(offsetField);

        if (offsetFieldOwner.getTemplate() != null)
        {
            final CompoundType offsetFieldOwnerTemplate = (CompoundType)offsetFieldOwner.getTemplate();
            final Field templateOffsetField =
                    offsetFieldOwnerTemplate.getFields().get(offsetFieldOwner.getFields().indexOf(offsetField));
            offsetFields.add(templateOffsetField);
        }
    }

    private void addCompoundsWithOffset(List<CompoundType> comopundTypeStack)
    {
        for (CompoundType compoundType : compoundTypeStack)
        {
            compoundsWithOffset.add(compoundType);
            if (compoundType.getTemplate() != null)
                compoundsWithOffset.add((CompoundType)compoundType.getTemplate());
        }
    }

    private boolean firstRun = true;
    private final List<CompoundType> compoundTypeStack = new ArrayList<CompoundType>();
    private final Set<CompoundType> compoundsWithOffset = new HashSet<CompoundType>();
    private final Set<Field> offsetFields = new HashSet<Field>();
};
