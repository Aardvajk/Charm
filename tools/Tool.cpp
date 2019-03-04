#include "Tool.h"

#include "actions/Action.h"

Tool::Tool(Settings *settings, QObject *parent)
    : QObject(parent),
      settings(settings)
{
}

void
Tool::focusGained()
{
}

void
Tool::focusLost()
{
}

void
Tool::enableRelatedActions(bool state)
{
    foreach(Action *action, relatedActions)
    {
        action->setEnabled(state);
    }
}

void
Tool::savingSettings()
{
    saveSettings(settings);
}

void
Tool::addRelatedAction(Action *action)
{
    relatedActions.append(action);
}

void
Tool::saveSettings(Settings *settings) const
{
}
