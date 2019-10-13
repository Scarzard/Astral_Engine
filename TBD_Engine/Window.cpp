#include "Window.h"

#include "mmgr/mmgr.h"

Window::Window() {}

Window::~Window() {}

bool Window::Start() { return true; }
bool Window::Draw() { return true; }
update_status Window::PreUpdate(float dt) { return UPDATE_CONTINUE; }
update_status Window::Update(float dt) { return UPDATE_CONTINUE; }
update_status Window::PostUpdate(float dt) { return UPDATE_CONTINUE; }
bool Window::CleanUp() { return true; }