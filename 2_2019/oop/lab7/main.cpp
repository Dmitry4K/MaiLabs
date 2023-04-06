#include <array>
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

#include "sdl.h"
#include "imgui.h"
#include "tools.h"
#include "loader.h"
//#include "journal.h"
#include "canvas.h"


int main() {
    canvas canv;
  color figure_color{};
  sdl::renderer renderer("Editor");//�������� ������� � ������ Editor
  bool quit = false;//������ � ����������� ����������� �� ������
  std::unique_ptr<builder> active_builder = nullptr;// ������� ���������� ��������� �� ���������
  const int32_t file_name_length = 128;//������������ ����� �������� �����
  char file_name[file_name_length] = "";//��� �����
  int32_t remove_id = 0;
  while(!quit){
    //������� ������ ������ ����
    renderer.set_color(0, 0, 0);
    renderer.clear();
    
    sdl::event event;

    while(sdl::event::poll(event)){
      sdl::quit_event quit_event;//������� ������
      sdl::mouse_button_event mouse_button_event;//������� ������ ����
      if(event.extract(quit_event)){//���� ������� 
        quit = true;
        break;
      }else if(event.extract(mouse_button_event)){
        if(active_builder && mouse_button_event.button() == sdl::mouse_button_event::left && //���� ���� ��������� � ���� ������ ����
            mouse_button_event.type() == sdl::mouse_button_event::down){
          std::unique_ptr<figure> figure =//nullptr, ���� � ��������� �� 3 �������, ����� ������
            active_builder->add_vertex(vertex{mouse_button_event.x(), mouse_button_event.y()});//� ��������� ����������� ��������� �������
          if(figure){
              (*figure).set_color(figure_color);
            canv.add_figure(std::move(figure));//�������� � ������ �����
            active_builder = nullptr;
          }
        }
      }
    }

    for (int i = 0; i < canv.figures.size(); ++i) {
        if(canv.figures[i])
      canv.figures[i]->render(renderer);//������ ������
    }

    ImGui::Begin("Menu");
    if (ImGui::Button("New canvas")) {
        canv.figures.clear();
        // jl.figs = std::move(figures);
         //jl.push(0, nullptr);
    }
    ImGui::InputText("File name", file_name, file_name_length - 1);
    if(ImGui::Button("Save")){
      std::ofstream os(file_name);
      if(os){
          for (int i = 0; i < canv.figures.size();++i) {
          canv.figures[i]->save(os);
        }
      }
    }
    ImGui::SameLine();
    if (ImGui::Button("Load")) {
        std::ifstream is(file_name);
        if (is) {
             loader loader;
            canv.figures = loader.load(is);
            //jl.push(2, nullptr);
        }
    }

    ImGui::InputInt("R", &figure_color.r);
    ImGui::InputInt("G", &figure_color.g);
    ImGui::InputInt("B", &figure_color.b);
    if (ImGui::Button("Red")) {
       figure_color.set_color(255, 0, 0);
    }
    ImGui::SameLine();
    if (ImGui::Button("Green")) {
        figure_color.set_color(0, 255, 0);
    }
    ImGui::SameLine();
    if (ImGui::Button("Blue")) {
        figure_color.set_color(0, 0, 255);
    }

    if (ImGui::Button("Line")) {
        active_builder = std::make_unique<line_builder>();
    }
    ImGui::SameLine();
    if(ImGui::Button("Trapeze")){
      active_builder = std::make_unique<trapeze_builder>();
    }

    ImGui::SameLine();
    if (ImGui::Button("Rhombus")) {
        active_builder = std::make_unique<rhombus_builder>();
    }
    ImGui::SameLine();
    if (ImGui::Button("Pentagon")) {
        active_builder = std::make_unique<pentagon_builder>();
    }
    ImGui::SameLine();
    if (ImGui::Button("Curve Line")) {
        active_builder = std::make_unique<curve_line_builder>();
    }


    ImGui::InputInt("Remove id", &remove_id);
    if(ImGui::Button("Remove")){
        if (remove_id < canv.figures.size()) {
            if(canv.figures[remove_id])
            canv.remove_figure(remove_id);
            //jl.push(-1, std::move(*(figures.begin() + remove_id)));
            //figures.erase(figures.begin() + remove_id);
        }
    }

    if (ImGui::Button("Undo")) {
        canv.undo();
    }
    ImGui::End();

    renderer.present();
  }
}
