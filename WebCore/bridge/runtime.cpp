/*
 * Copyright (C) 2003, 2006, 2008 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#include "config.h"
#include "runtime.h"

#include "runtime_object.h"
#include "runtime_root.h"

#if PLATFORM(QT)
#include "qt_instance.h"
#endif

namespace KJS { namespace Bindings {

Array::Array(PassRefPtr<RootObject> rootObject)
    : _rootObject(rootObject)
{
    ASSERT(_rootObject);
}

Array::~Array()
{
}

Instance::Instance(PassRefPtr<RootObject> rootObject)
    : _rootObject(rootObject)
{
    ASSERT(_rootObject);
}

Instance::~Instance()
{
}

static KJSDidExecuteFunctionPtr s_didExecuteFunction;

void Instance::setDidExecuteFunction(KJSDidExecuteFunctionPtr func)
{
    s_didExecuteFunction = func;
}

KJSDidExecuteFunctionPtr Instance::didExecuteFunction()
{
    return s_didExecuteFunction;
}

static JSGlobalObject* s_currentGlobalObject;

void Instance::setCurrentGlobalObject(JSGlobalObject* globalObject)
{
    s_currentGlobalObject = globalObject;
}

JSGlobalObject* Instance::currentGlobalObject()
{
    return s_currentGlobalObject;
}

void Instance::begin()
{
    setCurrentGlobalObject(_rootObject && _rootObject->isValid() ? _rootObject->globalObject() : 0);
    virtualBegin();
}

void Instance::end()
{
    virtualEnd();
}

JSValue *Instance::getValueOfField(ExecState *exec, const Field *aField) const
{
    return aField->valueFromInstance(exec, this);
}

void Instance::setValueOfField(ExecState *exec, const Field *aField, JSValue *aValue) const
{
    aField->setValueToInstance(exec, this, aValue);
}

JSObject* Instance::createRuntimeObject(PassRefPtr<Instance> instance)
{
#if PLATFORM(QT)
    if (instance->getBindingLanguage() == QtLanguage)
        return QtInstance::getRuntimeObject(static_cast<QtInstance*>(instance.get()));
#endif
    JSLock lock;

    return new RuntimeObjectImp(instance);
}

Instance* Instance::getInstance(JSObject* object, BindingLanguage language)
{
    if (!object)
        return 0;
    if (!object->inherits(&RuntimeObjectImp::info))
        return 0;
    Instance* instance = (static_cast<RuntimeObjectImp*>(object))->getInternalInstance();
    if (!instance)
        return 0;
    if (instance->getBindingLanguage() != language)
        return 0;
    return instance;
}

RootObject* Instance::rootObject() const 
{ 
    return _rootObject && _rootObject->isValid() ? _rootObject.get() : 0;
}

} } // namespace KJS::Bindings
