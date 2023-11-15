#include "Application.h"

std::string Application::generateRandomPassword(int length)
{
	const std::string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	std::random_device rd;
	std::mt19937 generator(rd());
	std::uniform_int_distribution<int> distribution(0, characters.size() - 1);

	std::string password;
	for (int i = 0; i < length; ++i) {
		password += characters[distribution(generator)];
	}

	return password;
}

void Application::filterText(std::string& value)
{
	std::replace(value.begin(), value.end(), ' ', '_');
}

Application::Application()
{
	passwordList = new Database();

	loggedIn = false;
	justLoggedIn = false;
	wrongPassCount = 0;

	password = "";

	addedPassType = "";
	addedPassText = "";
	foundPassText = "";
	deletedPassText = "";

	accountSet = "";
	passwordSet = "";
	accountGet = "";
	accountDelete = "";

	useMain = true;
}

void Application::Update()
{
	if (loggedIn)
	{
		ImGui::Begin("Passlist");
		// Calculate the center of the window
		ImVec2 windowSize = ImGui::GetCurrentWindow()->Size;
		ImVec2 center = ImVec2(windowSize.x * 0.5f, windowSize.y * 0.5f);

		std::map<std::string, DataItem*> data = passwordList->GetData();
		std::map<std::string, DataItem*>::iterator it = data.begin();
		//Read one line at a time
		ImGui::SetWindowFontScale(1.3f);
		int i = 0;
		while (it != data.end()) 
		{
			std::string Key = it->first;
			DataItem* dataItem = data[Key];

			//std::cout << Key << ' ' << dataItem->Value << '\n';

			ImVec2 textSize = ImGui::CalcTextSize(Key.c_str());
			ImVec2 textPosition = ImVec2(center.x - 425.f, (textSize.y) * i * 1.8 + center.y * 0.1);
			ImGui::SetCursorPos(textPosition);
			static char keyBuffer[26] = "";
			for (size_t i = 0; i < sizeof(keyBuffer); i++)
			{
				keyBuffer[i] = Key[i];
			}
			textSize = ImGui::CalcTextSize(Key.c_str());
			ImGui::SetNextItemWidth(textSize.x + 8);
			ImGui::InputText(std::string("##keylabel" + std::to_string(i)).c_str(), keyBuffer, sizeof(keyBuffer), ImGuiInputTextFlags_ReadOnly);

			ImGui::SameLine();
			textPosition = ImVec2(center.x - 175.f, (textSize.y) * i * 1.8 + center.y * 0.1);
			ImGui::SetCursorPos(textPosition);
			static char foundPassBuffer[65] = "";
			for (size_t i = 0; i < sizeof(foundPassBuffer); i++)
			{
				foundPassBuffer[i] = dataItem->Value[i];
			}
			textSize = ImGui::CalcTextSize(dataItem->Value.c_str());
			ImGui::SetNextItemWidth(textSize.x + 8);
			ImGui::InputText(std::string("##label" + std::to_string(i)).c_str(), foundPassBuffer, sizeof(foundPassBuffer), ImGuiInputTextFlags_ReadOnly);
			++it;
			i++;
		}
		//ImGui::SetWindowFontScale(1.f);
		ImGui::End();

		ImGui::Begin("SetPass");
		ImGui::SetWindowFontScale(1.3f);

		ImVec2 textSize = ImGui::CalcTextSize("Account:");
		ImVec2 textSize2 = ImGui::CalcTextSize("Password:");
		ImVec2 newPassPosition = ImVec2(center.x - (textSize.x/2 + textSize2.x /2 + center.x * 0.2), center.y - textSize.y * 4);
		ImGui::SetCursorPos(newPassPosition);
		ImGui::Text("Account:");

		ImGui::SameLine();
		ImGui::SetNextItemWidth(center.x * 0.2);
		static char nameBuffer[26] = "";
		for (size_t i = 0; i < sizeof(nameBuffer); i++)
		{
			nameBuffer[i] = accountSet[i];
		}
		ImGui::InputText("##namelabel", nameBuffer, sizeof(nameBuffer));
		accountSet = std::string(nameBuffer);
		filterText(accountSet);

		ImGui::SameLine();
		ImGui::Text("Password:");

		ImGui::SameLine();
		ImGui::SetNextItemWidth(center.x * 0.2);
		static char passwordBuffer[65] = "";
		for (size_t i = 0; i < sizeof(passwordBuffer); i++)
		{
			passwordBuffer[i] = passwordSet[i];
		}
		ImGui::InputText("##password", passwordBuffer, sizeof(passwordBuffer));
		passwordSet = std::string(passwordBuffer);
		filterText(passwordSet);

		textSize = ImGui::CalcTextSize("New Password");
		ImVec2 textPosition = ImVec2(center.x - textSize.x * 0.5f, center.y - textSize.y * 1.7);
		ImGui::SetCursorPos(textPosition);
		if (ImGui::Button("New Password"))
		{
			//std::cout << "Updated password: " << std::string(nameBuffer) << '\n';

			if (std::string(passwordBuffer) == "")
			{
				std::string generatedPassword = generateRandomPassword(64);
				passwordList->SetString(std::string(nameBuffer), generatedPassword);
				passwordList->SaveData("data.rndp", password);
				//std::cout << "Generated password: " << nameBuffer << '\n';
				addedPassType = "Generated password:";
				addedPassText = std::string(generatedPassword);
			}
			else
			{
				passwordList->SetString(std::string(nameBuffer), std::string(passwordBuffer));
				passwordList->SaveData("data.rndp", password);
				addedPassType = "Set password:";
				addedPassText = std::string(passwordBuffer);
			}
		}

		if (addedPassText != "")
		{
			textSize = ImGui::CalcTextSize(std::string(addedPassType + addedPassText).c_str());
			textPosition = ImVec2(center.x - textSize.x * 0.5f, center.y + textSize.y * 0.4);
			ImGui::SetCursorPos(textPosition);
			ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), addedPassType.c_str());

			ImGui::SameLine();
			static char newPassBuffer[65] = "";
			for (size_t i = 0; i < sizeof(newPassBuffer); i++)
			{
				newPassBuffer[i] = addedPassText[i];
			}
			textSize = ImGui::CalcTextSize(addedPassText.c_str());
			ImGui::SetNextItemWidth(textSize.x + 8);
			ImGui::InputText("##newPasslabel", newPassBuffer, sizeof(newPassBuffer), ImGuiInputTextFlags_ReadOnly);
		}

		ImGui::End();

		ImGui::Begin("GetPass");
		ImGui::SetWindowFontScale(1.3f);

		textSize = ImGui::CalcTextSize("Account:");
		newPassPosition = ImVec2(center.x - (textSize.x / 2 + textSize2.x / 2 + center.x * 0.2 / 2), center.y - textSize.y * 4);
		ImGui::SetCursorPos(newPassPosition);
		ImGui::Text("Find password:");

		ImGui::SameLine();
		ImGui::SetNextItemWidth(center.x * 0.2);
		
		static char nameBuffer2[25] = "";
		for (size_t i = 0; i < sizeof(nameBuffer2); i++)
		{
			nameBuffer2[i] = accountGet[i];
		}
		ImGui::InputText("##namelabel2", nameBuffer2, sizeof(nameBuffer2));
		accountGet = std::string(nameBuffer2);
		filterText(accountGet);

		textSize = ImGui::CalcTextSize("Find");
		textPosition = ImVec2(center.x - textSize.x * 0.5f, center.y - textSize.y * 1.7);
		ImGui::SetCursorPos(textPosition);
		if (ImGui::Button("Find"))
		{
			std::string pass = passwordList->GetString(nameBuffer2);
			foundPassText = pass;
		}

		if (foundPassText != "")
		{
			textSize = ImGui::CalcTextSize(std::string("Password:" + foundPassText).c_str());
			textPosition = ImVec2(center.x - textSize.x * 0.5f, center.y + textSize.y * 0.4);
			ImGui::SetCursorPos(textPosition);
			ImGui::TextColored(ImVec4(0.0f, 1.f, 1.0f, 1.0f), "Password:");

			ImGui::SameLine();
			static char foundPassBuffer[65] = "";
			for (size_t i = 0; i < sizeof(foundPassBuffer); i++)
			{
				foundPassBuffer[i] = foundPassText[i];
			}
			textSize = ImGui::CalcTextSize(foundPassText.c_str());
			ImGui::SetNextItemWidth(textSize.x + 8);
			ImGui::InputText("##foundPasslabel", foundPassBuffer, sizeof(foundPassBuffer), ImGuiInputTextFlags_ReadOnly);
		}

		ImGui::End();

		ImGui::Begin("DeletePass");
		ImGui::SetWindowFontScale(1.3f);

		textSize = ImGui::CalcTextSize("Account:");
		newPassPosition = ImVec2(center.x - (textSize.x / 2 + textSize2.x / 2 + center.x * 0.2 / 2), center.y - textSize.y * 4);
		ImGui::SetCursorPos(newPassPosition);
		ImGui::Text("Delete password:");

		ImGui::SameLine();
		ImGui::SetNextItemWidth(center.x * 0.2);

		static char nameBuffer3[26] = "";
		for (size_t i = 0; i < sizeof(nameBuffer3); i++)
		{
			nameBuffer3[i] = accountDelete[i];
		}
		ImGui::InputText("##namelabel2", nameBuffer3, sizeof(nameBuffer3));
		accountDelete = std::string(nameBuffer3);
		filterText(accountDelete);

		textSize = ImGui::CalcTextSize("Delete");
		textPosition = ImVec2(center.x - textSize.x * 0.5f, center.y - textSize.y * 1.7);
		ImGui::SetCursorPos(textPosition);
		if (ImGui::Button("Delete"))
		{
			if (passwordList->DeleteData(nameBuffer3))
			{
				passwordList->SaveData("data.rndp", password);
				deletedPassText = "Deleted password:" + std::string(nameBuffer3);
			}
			else
			{
				deletedPassText = "Password: Not found";
			}
		}

		textSize = ImGui::CalcTextSize(deletedPassText.c_str());
		textPosition = ImVec2(center.x - textSize.x * 0.5f, center.y + textSize.y * 0.4);
		ImGui::SetCursorPos(textPosition);
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), deletedPassText.c_str());

		ImGui::End();

		if (!justLoggedIn)
		{
			justLoggedIn = true;
			ImGui::SetWindowFocus("Passlist");
		}
	}
	else
	{
		// Calculate the center of the window
		ImVec2 windowSize = ImGui::GetWindowSize();
		ImVec2 center = ImVec2(windowSize.x * 0.5f, windowSize.y * 0.5f);

		// Set the font scale to increase text size
		ImGui::SetWindowFontScale(1.3f);
		// Calculate the width of the text
		//const char* text = "Randpass";
		const char* text = "   ___               __                \n"
			"  / _ \\___ ____  ___/ /__  ___ ____ ___\n"
			" / , _/ _ `/ _ \\/ _  / _ \\/ _ `(_-<(_-<\n"
			"/_/|_|\\_,_/_//_/\\_,_/ .__/\\_,_/___/___/\n"
			"                   /_/                 \n";
		ImVec2 textSize = ImGui::CalcTextSize(" / , _/ _ `/ _ \\/ _  / _ \\/ _ `(_-<(_-<");
		// Calculate the position to center the text
		ImVec2 textPosition = ImVec2(center.x - textSize.x * 0.5f, center.y - textSize.y * 7);
		// Set the cursor position to center the text
		ImGui::SetCursorPos(textPosition);
		// Add the centered text
		ImGui::Text("%s", text);
		//ImGui::SetWindowFontScale(1.3f);

		textSize = ImGui::CalcTextSize("Password:");
		ImGui::SetCursorPos(ImVec2(center.x * 0.45, center.y - textSize.y * 1 + textSize.y / 2));
		ImGui::Text("Password:");

		ImGui::SameLine();
		ImGui::SetNextItemWidth(center.x * 0.9);
		// Create an input box
		static char inputBuffer[256] = "";
		ImGui::InputText("##inputlabel", inputBuffer, sizeof(inputBuffer));
		//ImGui::Text(inputBuffer);
		// Check for Enter key press
		if (ImGui::IsKeyPressed(ImGuiKey_Enter, false)) 
		{
			loggedIn = passwordList->LoadData("data.rndp", std::string(inputBuffer));
			if (loggedIn)
			{
				useMain = false;
				password = std::string(inputBuffer);
			}
			else
			{
				wrongPassCount++;
			}
		}

		if (wrongPassCount > 0)
		{
			// Display the entered text
			ImGui::SetCursorPos(ImVec2(center.x * 0.45, center.y + textSize.y * 1));
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Wrong password(%d)", wrongPassCount);
		}
	}
}
