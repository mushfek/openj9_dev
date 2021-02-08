/**
 * @author Mushfekur Rahman (rahman.mushfek@gmail.com)
 * @date 2021-02-06
 */

#ifndef STATIC_ANALYZER_H
#define STATIC_ANALYZER_H

#include "j9.h"
#include "j9port.h"
#include "j9protos.h"
#include "j9consts.h"

typedef struct StaticAnalyzerContext {
    J9VMThread* jvmThread;
    J9ROMClass* romClass;
    J9ClassLoader* classLoader;
} StaticAnalyzerContext;

void perform_static_analysis(StaticAnalyzerContext* analyzerContext);

#endif /* STATIC_ANALYZER_H */
