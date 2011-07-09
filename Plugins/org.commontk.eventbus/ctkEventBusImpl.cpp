/*=============================================================================

  Library: CTK

  Copyright (c) German Cancer Research Center,
    Division of Medical and Biological Informatics

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

=============================================================================*/

#include "ctkEventBusImpl_p.h"

#include <QSetIterator>

#include "ctkEventHandlerWrapper_p.h"


ctkEventBusImpl* ctkEventBusImpl::instance()
{
  static ctkEventBusImpl inst;
  return &inst;
}

ctkEventBusImpl::ctkEventBusImpl()
{

}

void ctkEventBusImpl::postEvent(const ctkEvent& event)
{
  dispatchEvent(event, true);
}

void ctkEventBusImpl::sendEvent(const ctkEvent& event)
{
  dispatchEvent(event, false);
}

void ctkEventBusImpl::publishSignal(const QObject* publisher, const char* signal, const QString& topic,
                                    Qt::ConnectionType type)
{
  Q_UNUSED(publisher)
  Q_UNUSED(signal)
  Q_UNUSED(topic)
  Q_UNUSED(type)
}

QString ctkEventBusImpl::subscribeSlot(const QObject* subscriber, const char* member, const ctkProperties& properties)
{

  // TODO check for duplicates

  ctkEventHandlerWrapper* wrapper = new ctkEventHandlerWrapper(subscriber, member, properties);
  if (wrapper->init())
  {
    bucket(wrapper);
  }

  // TODO return id
  return QString();
}

void ctkEventBusImpl::updateProperties(const QString& subscriptionId, const ctkProperties& properties)
{
  Q_UNUSED(subscriptionId)
  Q_UNUSED(properties)
}

void ctkEventBusImpl::dispatchEvent(const ctkEvent& event, bool isAsync)
{
  Q_UNUSED(isAsync)

  QString topic = event.topic();

  QSet<ctkEventHandlerWrapper*> eventHandlers = this->handlers(topic);
  if (eventHandlers.empty()) return;

  QSetIterator<ctkEventHandlerWrapper*> iter(eventHandlers);
  while (iter.hasNext())
  {
    iter.next()->handleEvent(event);
  }
}

void ctkEventBusImpl::bucket(ctkEventHandlerWrapper* wrapper)
{
  // TODO bucket logic
  globalWildcard.push_back(wrapper);
}

QSet<ctkEventHandlerWrapper*> ctkEventBusImpl::handlers(const QString& topic)
{
  Q_UNUSED(topic)

  // TODO
  return globalWildcard.toSet();
}

