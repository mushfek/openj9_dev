/**
 * @author Mushfekur Rahman (rahman.mushfek@gmail.com)
 * @brief A simple static analyzer that prints the method signatures (with type) 
 *        of application classes with more than three parameters
 * @date 2021-02-06
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "string_builder.h"
#include "static_analyzer.h"

void perform_static_analysis(StaticAnalyzerContext *analyzerContext);
void print_method_signature(J9ROMMethod *romMethod, J9VMThread *currentThread);
void pretty_print_method_signature(J9ROMMethod *romMethod, J9VMThread *currentThread);

/**
 * @brief Entry point of the static analysis
 * Walks the ROMClass to find methods that has more than three parameters and prints it
 */
void perform_static_analysis(StaticAnalyzerContext *analyzerContext)
{
    J9VMThread *currentThread = analyzerContext->jvmThread;
    J9ROMClass *romClass = analyzerContext->romClass;
    J9ClassLoader *classLoader = analyzerContext->classLoader;

    J9JavaVM *vm = currentThread->javaVM;

    /**
	 * Analyze only application classes (e.g. not bootstrap and extension lib classes)
	 */
    bool isApplicationClassLoader = classLoader == vm->applicationClassLoader;
    if (!isApplicationClassLoader)
    {
        return;
    }

    J9UTF8 *className = J9ROMCLASS_CLASSNAME(romClass);

    J9ROMMethod *romMethod = J9ROMCLASS_ROMMETHODS(romClass);
    for (U_32 i = 0; i < romClass->romMethodCount; i++)
    {

        /* Print method signature if it has more than three parameters */
        if (romMethod->argCount > 3)
        {
            print_method_signature(romMethod, currentThread);
        }

        romMethod = nextROMMethod(romMethod);
    }
}

void print_method_signature(J9ROMMethod *romMethod, J9VMThread *currentThread)
{
    PORT_ACCESS_FROM_VMC(currentThread);

    J9UTF8 *methodName = J9ROMMETHOD_NAME(romMethod);
    J9UTF8 *methodSignature = J9ROMMETHOD_SIGNATURE(romMethod);

    j9tty_printf(PORTLIB, "[Static Analyzer] Method Signature (VM): %s%s\n", J9UTF8_DATA(methodName), J9UTF8_DATA(methodSignature));
    j9tty_printf(PORTLIB, "[Static Analyzer] Method Signature (Pretty): ");

    pretty_print_method_signature(romMethod, currentThread);
}

void pretty_print_method_signature(J9ROMMethod *romMethod, J9VMThread *currentThread)
{
    PORT_ACCESS_FROM_VMC(currentThread);

    str_builder_t *signature_sb = str_builder_init();

    /* Access Modifiers */
    if (romMethod->modifiers & CFR_ACC_PUBLIC)
        str_builder_add_str(signature_sb, "public ");
    if (romMethod->modifiers & CFR_ACC_PRIVATE)
        str_builder_add_str(signature_sb, "private ");
    if (romMethod->modifiers & CFR_ACC_PROTECTED)
        str_builder_add_str(signature_sb, "protected ");
    if (romMethod->modifiers & CFR_ACC_STATIC)
        str_builder_add_str(signature_sb, "static ");
    if (romMethod->modifiers & CFR_ACC_FINAL)
        str_builder_add_str(signature_sb, "final ");
    if (romMethod->modifiers & CFR_ACC_SYNCHRONIZED)
        str_builder_add_str(signature_sb, "synchronized ");
    if (romMethod->modifiers & CFR_ACC_NATIVE)
        str_builder_add_str(signature_sb, "native ");
    if (romMethod->modifiers & CFR_ACC_ABSTRACT)
        str_builder_add_str(signature_sb, "abstract ");
    if (romMethod->modifiers & CFR_ACC_STRICT)
        str_builder_add_str(signature_sb, "strict ");

    /* Method Name */
    J9UTF8 *methodName = J9ROMMETHOD_NAME(romMethod);
    J9UTF8 *methodSignature = J9ROMMETHOD_SIGNATURE(romMethod);
    U_8 *signature_str = J9UTF8_DATA(methodSignature);

    /* Return Type */
    U_32 i = 0;
    while (signature_str[i++] != ')')
        ;

    U_32 dim = 0;
    while (signature_str[i] == '[')
        dim++, i++;

    switch (signature_str[i])
    {
    case 'B':
        str_builder_add_str(signature_sb, "byte");
        break;

    case 'C':
        str_builder_add_str(signature_sb, "char");
        break;

    case 'D':
        str_builder_add_str(signature_sb, "double");
        break;

    case 'F':
        str_builder_add_str(signature_sb, "float");
        break;

    case 'I':
        str_builder_add_str(signature_sb, "int");
        break;

    case 'J':
        str_builder_add_str(signature_sb, "long");
        break;

    case 'L':
        i++;
        while (signature_str[i] != ';')
        {
            str_builder_add_char(signature_sb, (signature_str[i] == '/') ? '.' : signature_str[i]);
            i++;
        }
        break;

    case 'S':
        str_builder_add_str(signature_sb, "short");
        break;

    case 'V':
        str_builder_add_str(signature_sb, "void");
        break;

    case 'Z':
        str_builder_add_str(signature_sb, "boolean");
        break;
    }

    for (i = 0; i < dim; i++)
        str_builder_add_str(signature_sb, "[]");

    str_builder_add_char(signature_sb, ' ');
    str_builder_add_str(signature_sb, J9UTF8_DATA(methodName));
    str_builder_add_char(signature_sb, '(');

    /* Parameter List (with types) */
    for (i = 1; signature_str[i] != ')'; i++)
    {
        dim = 0;
        while (signature_str[i] == '[')
            dim++, i++;

        switch (signature_str[i])
        {
        case 'B':
            str_builder_add_str(signature_sb, "byte");
            break;

        case 'C':
            str_builder_add_str(signature_sb, "char");
            break;

        case 'D':
            str_builder_add_str(signature_sb, "double");
            break;

        case 'F':
            str_builder_add_str(signature_sb, "float");
            break;

        case 'I':
            str_builder_add_str(signature_sb, "int");
            break;

        case 'J':
            str_builder_add_str(signature_sb, "long");
            break;

        case 'L':
            i++;
            while (signature_str[i] != ';')
            {
                str_builder_add_char(signature_sb, (signature_str[i] == '/') ? '.' : signature_str[i]);
                i++;
            }
            break;

        case 'S':
            str_builder_add_str(signature_sb, "short");
            break;

        case 'V':
            str_builder_add_str(signature_sb, "void");
            break;

        case 'Z':
            str_builder_add_str(signature_sb, "boolean");
            break;
        }

        U_32 j = 0;
        for (j = 0; j < dim; j++)
            str_builder_add_str(signature_sb, "[]");

        if (signature_str[i + 1] != ')')
            str_builder_add_str(signature_sb, ", ");
    }

    str_builder_add_str(signature_sb, ")\n");

    j9tty_printf(PORTLIB, signature_sb->result);

    str_builder_free(signature_sb);
}
