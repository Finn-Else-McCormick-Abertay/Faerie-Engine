#include "input_action.h"

ActionIdentifier::ActionIdentifier(const std::string& ns, const std::string& action) : m_namespace(ns), m_action(action) {}

std::string ActionIdentifier::Namespace() const { return m_namespace; }
std::string ActionIdentifier::Action() const { return m_action; }

std::string ActionIdentifier::AsString() const { return m_namespace + "::" + m_action; }



void ActionBinding::Bind(PhysicalKeyCode v)     { m_physicalKeys.insert(v); }
void ActionBinding::Bind(LogicalKeyCode v)      { m_logicalKeys.insert(v); }
void ActionBinding::Bind(ControllerButton v)    { m_controllerButtons.insert(v); }
void ActionBinding::Bind(MouseButton v)         { m_mouseButtons.insert(v); }
void ActionBinding::Bind(MouseMotionSource v)   { m_mouseMotionSources.insert(v); }
void ActionBinding::Bind(ControllerStick v)     { m_controllerSticks.insert(v); }
void ActionBinding::Bind(ControllerTrigger v)   { m_controllerTriggers.insert(v); }

void ActionBinding::Unbind(PhysicalKeyCode v)   { m_physicalKeys.erase(v); }
void ActionBinding::Unbind(LogicalKeyCode v)    { m_logicalKeys.erase(v); }
void ActionBinding::Unbind(ControllerButton v)  { m_controllerButtons.erase(v); }
void ActionBinding::Unbind(MouseButton v)       { m_mouseButtons.erase(v); }
void ActionBinding::Unbind(MouseMotionSource v) { m_mouseMotionSources.erase(v); }
void ActionBinding::Unbind(ControllerStick v)   { m_controllerSticks.erase(v); }
void ActionBinding::Unbind(ControllerTrigger v) { m_controllerTriggers.erase(v); }

bool ActionBinding::Has(PhysicalKeyCode v)      { m_physicalKeys.contains(v); }
bool ActionBinding::Has(LogicalKeyCode v)       { m_logicalKeys.contains(v); }
bool ActionBinding::Has(ControllerButton v)     { m_controllerButtons.contains(v); }
bool ActionBinding::Has(MouseButton v)          { m_mouseButtons.contains(v); }
bool ActionBinding::Has(MouseMotionSource v)    { m_mouseMotionSources.contains(v); }
bool ActionBinding::Has(ControllerStick v)      { m_controllerSticks.contains(v); }
bool ActionBinding::Has(ControllerTrigger v)    { m_controllerTriggers.contains(v); }