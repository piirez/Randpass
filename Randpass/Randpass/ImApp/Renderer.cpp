#include "Renderer.h"

void Renderer::StartDraw(bool useMain)
{
    // render
    // ------

    // feed inputs to dear imgui, start new frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGuiID dockspaceID = ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

    if (useMain)
    {
        ImGuiWindowClass window_class;
        window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
        ImGui::SetNextWindowClass(&window_class);
        ImGui::DockBuilderDockWindow("Main", dockspaceID);
        ImGui::Begin("Main");
    }
    else
    {
        ImGuiWindowClass window_class;
        ImGui::SetNextWindowClass(&window_class);
    }

    ImGui::DockBuilderDockWindow("Passlist", dockspaceID);
    ImGui::DockBuilderDockWindow("SetPass", dockspaceID);
    ImGui::DockBuilderDockWindow("GetPass", dockspaceID);
    ImGui::DockBuilderDockWindow("DeletePass", dockspaceID);
    ImGui::DockBuilderFinish(dockspaceID);
}

void Renderer::EndDraw(bool useMain)
{
    if (useMain)
        ImGui::End();

    // Render dear imgui into screen
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

void Renderer::CleanUp()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
