//
//  ScriptContext.h
//  libraries/script-engine/src
//
//  Created by Heather Anderson on 5/1/21.
//  Copyright 2021 Vircadia contributors.
//  Copyright 2023 Overte e.V.
//
//  Distributed under the Apache License, Version 2.0.
//  See the accompanying file LICENSE or http://www.apache.org/licenses/LICENSE-2.0.html
//  SPDX-License-Identifier: Apache-2.0
//

/// @addtogroup ScriptEngine
/// @{

#ifndef hifi_ScriptContext_h
#define hifi_ScriptContext_h

#include <memory>

#include <QtCore/QString>
#include <QtCore/QStringList>

#include "ScriptValue.h"

class ScriptContext;
class ScriptEngine;
class ScriptFunctionContext;
using ScriptContextPointer = std::shared_ptr<ScriptContext>;
using ScriptFunctionContextPointer = std::shared_ptr<ScriptFunctionContext>;
using ScriptEnginePointer = std::shared_ptr<ScriptEngine>;

/// [ScriptInterface] Provides an engine-independent interface for QScriptContextInfo
class ScriptFunctionContext {
public:
    enum FunctionType {
        ScriptFunction = 0,
        QtFunction = 1,
        QtPropertyFunction = 2,
        NativeFunction = 3,
    };
    
public:
    virtual QString fileName() const = 0;
    virtual QString functionName() const = 0;
    virtual FunctionType functionType() const = 0;
    virtual int lineNumber() const = 0;

protected:
    ~ScriptFunctionContext() {} // prevent explicit deletion of base class
};

/// [ScriptInterface] Provides an engine-independent interface for QScriptContext
class ScriptContext {
public:
    virtual int argumentCount() const = 0;
    virtual ScriptValue argument(int index) const = 0;
    virtual QStringList backtrace() const = 0;
    virtual ScriptValue callee() const = 0;
    virtual ScriptEnginePointer engine() const = 0;
    virtual ScriptFunctionContextPointer functionContext() const = 0;
    virtual ScriptContextPointer parentContext() const = 0;
    virtual ScriptValue thisObject() const = 0;
    virtual ScriptValue throwError(const QString& text) = 0;
    virtual ScriptValue throwValue(const ScriptValue& value) = 0;

protected:
    ~ScriptContext() {}  // prevent explicit deletion of base class
};

class ScriptContextGuard {
public:
    ScriptContextGuard(ScriptContext* context);
    ~ScriptContextGuard();

private:
    ScriptContext* _oldContext;
};

#endif // hifi_ScriptContext_h

/// @}
