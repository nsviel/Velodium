#ifndef CONSOL_STRUCT_H
#define CONSOL_STRUCT_H

#include "../../extern/imgui/imgui.h"

/**
 * \brief UI consol
 * \struct ConsoleApp struct_consol.h "UI consol structure"
 */

// Normally you would store more information in your item (e.g. make Items[] an array of structure, store color/type etc.)
//add a log :  AddLog("%d some text", Items.Size); OU AddLog("some more text");
//add an error log : AddLog("[error] something went wrong");
//clear: ClearLog();
struct ConsoleApp{
  char                  InputBuf[256];
  ImVector<char*>       Items;
  ImVector<const char*> Commands;
  bool                  AutoScroll;
  bool                  ScrollToBottom;

  ConsoleApp(){
    ClearLog();
    memset(InputBuf, 0, sizeof(InputBuf));
    Commands.push_back("HELP");
    Commands.push_back("CLEAR");
    AutoScroll = true;
    ScrollToBottom = false;
  }
  ~ConsoleApp(){
    ClearLog();
  }

  // Portable helpers
  static int   Stricmp(const char* str1, const char* str2)         { int d; while ((d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; } return d; }
  static int   Strnicmp(const char* str1, const char* str2, int n) { int d = 0; while (n > 0 && (d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; n--; } return d; }
  static char* Strdup(const char *str)                             { size_t len = strlen(str) + 1; void* buf = malloc(len); IM_ASSERT(buf); return (char*)memcpy(buf, (const void*)str, len); }
  static void  Strtrim(char* str)                                  { char* str_end = str + strlen(str); while (str_end > str && str_end[-1] == ' ') str_end--; *str_end = 0; }

  void NumberOfLog(){
    std::cout<<"Number of consol logs: "<<Items.Size<<std::endl;
  }
  void ClearLog(){
    for (int i = 0; i < Items.Size; i++){
      free(Items[i]);
    }
    Items.clear();
  }
  void AddLog(const char* fmt, ...) IM_FMTARGS(2){
    // FIXME-OPT
    char buf[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
    buf[IM_ARRAYSIZE(buf)-1] = 0;
    va_end(args);
    Items.push_back(Strdup(buf));
  }

  void Draw(){
    const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing(); // 1 separator, 1 input text
    ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar); // Leave room for 1 separator + 1 InputText
    if (ImGui::BeginPopupContextWindow()){
      if (ImGui::Selectable("Clear")) ClearLog();
      ImGui::EndPopup();
    }

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4,1)); // Tighten spacing
    for (int i = 0; i < Items.Size; i++){
      const char* item = Items[i];

      //Item color
      bool pop_color = false;
      if (strstr(item, "[error]")){
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.4f, 0.4f, 1.0f));
        pop_color = true;
      }
      if (strstr(item, "[sucess]")){
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.4f, 1.0f, 0.4f, 1.0f));
        pop_color = true;
      }
      if (strstr(item, "[ok]")){
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.4f, 0.8f, 0.4f, 1.0f));
        pop_color = true;
      }
      else if (strncmp(item, "# ", 2) == 0){
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.6f, 1.0f));
        pop_color = true;
      }
      ImGui::TextUnformatted(item);
      if (pop_color){
        ImGui::PopStyleColor();
      }
    }

    //Scrolling
    if (ScrollToBottom || (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())){
      ImGui::SetScrollHereY(1.0f);
    }
    ScrollToBottom = false;

    ImGui::PopStyleVar();
    ImGui::EndChild();
    ImGui::Separator();

    // Command-line
    bool reclaim_focus = false;
    if (ImGui::InputText("Input", InputBuf, IM_ARRAYSIZE(InputBuf), ImGuiInputTextFlags_EnterReturnsTrue|ImGuiInputTextFlags_CallbackCompletion|ImGuiInputTextFlags_CallbackHistory, &TextEditCallbackStub, (void*)this)){
      char* s = InputBuf;
      Strtrim(s);
      if (s[0]){
        ExecCommand(s);
      }
      strcpy(s, "");
      reclaim_focus = true;
    }

    // Auto-focus on window apparition
    ImGui::SetItemDefaultFocus();
    if(reclaim_focus){
      ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget
    }

    ImGui::End();
  }
  void ExecCommand(const char* command_line){
    AddLog("# %s\n", command_line);

    // Process command
    if (Stricmp(command_line, "CLEAR") == 0){
      ClearLog();
    }
    else if (Stricmp(command_line, "HELP") == 0){
      AddLog("Commands:");
      for (int i = 0; i < Commands.Size; i++)
        AddLog("- %s", Commands[i]);
    }
    else{
      AddLog("Unknown command: '%s'\n", command_line);
    }

    // On commad input, we scroll to bottom even if AutoScroll==false
    ScrollToBottom = true;
  }
  static int TextEditCallbackStub(ImGuiInputTextCallbackData* data){
    ConsoleApp* console = (ConsoleApp*)data->UserData;
    return console->TextEditCallback(data);
  }
  int TextEditCallback(ImGuiInputTextCallbackData* data){
    //AddLog("cursor: %d, selection: %d-%d", data->CursorPos, data->SelectionStart, data->SelectionEnd);
    switch (data->EventFlag){
      case ImGuiInputTextFlags_CallbackCompletion:{
        // Example of TEXT COMPLETION

        // Locate beginning of current word
        const char* word_end = data->Buf + data->CursorPos;
        const char* word_start = word_end;
        while (word_start > data->Buf){
          const char c = word_start[-1];
          if (c == ' ' || c == '\t' || c == ',' || c == ';')
              break;
          word_start--;
        }

        // Build a list of candidates
        ImVector<const char*> candidates;
        for (int i = 0; i < Commands.Size; i++)
          if (Strnicmp(Commands[i], word_start, (int)(word_end-word_start)) == 0)
            candidates.push_back(Commands[i]);

        if (candidates.Size == 0){
          // No match
          AddLog("No match for \"%.*s\"!\n", (int)(word_end-word_start), word_start);
        }
        else if (candidates.Size == 1){
          // Single match. Delete the beginning of the word and replace it entirely so we've got nice casing
          data->DeleteChars((int)(word_start-data->Buf), (int)(word_end-word_start));
          data->InsertChars(data->CursorPos, candidates[0]);
          data->InsertChars(data->CursorPos, " ");
        }
        else{
          // Multiple matches. Complete as much as we can, so inputing "C" will complete to "CL" and display "CLEAR" and "CLASSIFY"
          int match_len = (int)(word_end - word_start);
          for (;;){
            int c = 0;
            bool all_candidates_matches = true;
            for (int i = 0; i < candidates.Size && all_candidates_matches; i++){
              if (i == 0){
                c = toupper(candidates[i][match_len]);
              }
              else if (c == 0 || c != toupper(candidates[i][match_len])){
                all_candidates_matches = false;
              }
            }
            if (!all_candidates_matches){
              break;
            }
            match_len++;
          }

          if (match_len > 0){
            data->DeleteChars((int)(word_start - data->Buf), (int)(word_end-word_start));
            data->InsertChars(data->CursorPos, candidates[0], candidates[0] + match_len);
          }

          // List matches
          AddLog("Possible matches:\n");
          for (int i = 0; i < candidates.Size; i++){
            AddLog("- %s\n", candidates[i]);
          }
        }

        break;
      }
    }
    return 0;
  }
};

#endif
