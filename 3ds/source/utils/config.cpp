/*
*   This file is part of BlockEscape3DS
*   Copyright (C) 2020 Universal-Team
*
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*   Additional Terms 7.b and 7.c of GPLv3 apply to this file:
*       * Requiring preservation of specified reasonable legal notices or
*         author attributions in that material or in the Appropriate Legal
*         Notices displayed by works containing it.
*       * Prohibiting misrepresentation of the origin of that material,
*         or requiring that modified versions of such material be marked in
*         reasonable ways as different from the original version.
*/

#include "colorHelper.hpp"
#include "common.hpp"
#include "config.hpp"

#include <3ds.h>
#include <string>
#include <unistd.h>

#define SETTINGS_FILE "sdmc:/3ds/BlockEscape3DS/Settings.json"

// Colors for default Blocks.
static u32 getColor(Blocks bl) {
	switch(bl) {
		case Blocks::Lock_Invalid:
			return 0;
		case Blocks::Lock1:
			return C2D_Color32(1, 199, 24, 255);
		case Blocks::Lock2:
			return C2D_Color32(248, 138, 15, 255);
		case Blocks::Lock3:
			return C2D_Color32(4, 173, 194, 255);
		case Blocks::Lock4:
			return C2D_Color32(133, 136, 119, 255);
		case Blocks::Lock5:
			return C2D_Color32(228, 135, 145, 255);
		case Blocks::Lock6:
			return C2D_Color32(231, 193, 144, 255);
		case Blocks::Lock7:
			return C2D_Color32(143, 105, 86, 255);
		case Blocks::Lock8:
			return C2D_Color32(223, 212, 106, 255);
		case Blocks::Lock9:
			return C2D_Color32(126, 219, 162, 255);
		case Blocks::Lock10:
			return C2D_Color32(117, 119, 180, 255);
		case Blocks::Lock11:
			return C2D_Color32(145, 145, 33, 255);
		case Blocks::Lock12:
			return C2D_Color32(178, 143, 163, 255);
		case Blocks::Lock13:
			return C2D_Color32(5, 123, 151, 255);
		case Blocks::Lock14:
			return C2D_Color32(253, 209, 42, 255);
		case Blocks::Lock15:
			return C2D_Color32(7, 179, 97, 255);
		case Blocks::Escape:
			return C2D_Color32(248, 5, 0, 255);
	}

	return 0;
}

// Used to add missing stuff for the JSON.
void Config::addMissingThings() { }

// In case it doesn't exist.
void Config::initialize() {
	// Create through fopen "Write".
	FILE *file = fopen(SETTINGS_FILE, "w");

	// Set default values.
	this->setInt("Bar_Color", BAR_COLOR);
	this->setInt("Text_Color", TEXT_COLOR);
	this->setInt("BG_Color", BG_COLOR);
	this->setInt("Button_Color", BUTTON_COLOR);
	this->setInt("Selector_Color", SELECTED_COLOR);

	// Block Colors.
	for (int i = 1; i < 17; i++) {
		this->setInt("Block_" + std::to_string(i), getColor((Blocks(i))));
	}

	this->setInt("Version", this->configVersion);
	this->setInt("Language", 2);

	// Write to file.
	std::string dump = this->json.dump(1, '\t');
	fwrite(dump.c_str(), 1, this->json.dump(1, '\t').size(), file);
	fclose(file); // Now we have the file and can properly access it.
}

Config::Config() {
	if (access(SETTINGS_FILE, F_OK) != 0 ) {
		this->initialize();
	}

	FILE* file = fopen(SETTINGS_FILE, "r");
	this->json = nlohmann::json::parse(file, nullptr, false);
	fclose(file);

	if (!this->json.contains("Version")) {
		// Let us create a new one.
		this->initialize();
	}

	// Here we add the missing things.
	if (this->json["Version"] < this->configVersion) {
		this->addMissingThings();
	}

	if (!this->json.contains("Bar_Color")) {
		this->barColor(BAR_COLOR);
	} else {
		this->barColor(this->getInt("Bar_Color"));
	}

	if (!this->json.contains("BG_Color")) {
		this->bgColor(BG_COLOR);
	} else {
		this->bgColor(this->getInt("BG_Color"));
	}

	if (!this->json.contains("Text_Color")) {
		this->textColor(TEXT_COLOR);
	} else {
		this->textColor(this->getInt("Text_Color"));
	}

	if (!this->json.contains("Button_Color")) {
		this->buttonColor(BUTTON_COLOR);
	} else {
		this->buttonColor(this->getInt("Button_Color"));
	}

	if (!this->json.contains("Selector_Color")) {
		this->selectorColor(SELECTED_COLOR);
	} else {
		this->selectorColor(this->getInt("Selector_Color"));
	}


	for (int i = 1; i < 17; i++) {
		if (!this->json.contains("Block_" + std::to_string(i))) {
			this->blockColor(i - 1, getColor(Blocks(i)));
		} else {
			this->blockColor(i - 1, this->getInt("Block_" + std::to_string(i)));
		}
	}

	if (!this->json.contains("Debug")) {
		this->v_debug = false;
	} else {
		this->v_debug = this->getBool("Debug");
	}

	if (!this->json.contains("Language")) {
		this->language(2);
	} else {
		this->language(this->getInt("Language"));
	}

	if (!this->json.contains("Version")) {
		this->version(this->configVersion);
	} else {
		this->version(this->getInt("Version"));
	}

	this->changesMade = false; // No changes made yet.
}

// Write to config if changesMade.
void Config::save() {
	if (this->changesMade) {
		FILE *file = fopen(SETTINGS_FILE, "w");

		// Set values.
		this->setInt("Bar_Color", this->barColor());
		this->setInt("Text_Color", this->textColor());
		this->setInt("BG_Color", this->bgColor());
		this->setInt("Button_Color", this->buttonColor());
		this->setInt("Selector_Color", this->selectorColor());

		for (int i = 1; i < 17; i++) {
			this->setInt("Block_" + std::to_string(i), this->blockColor(i - 1));
		}

		this->setInt("Language", this->language());
		this->setInt("Version", this->configVersion);

		// Write changes to file.
		std::string dump = this->json.dump(1, '\t');
		fwrite(dump.c_str(), 1, this->json.dump(1, '\t').size(), file);
		fclose(file);
	}
}

// Helper functions.
bool Config::getBool(const std::string &key) {
	if (!this->json.contains(key)) {
		return false;
	}
	
	return this->json.at(key).get_ref<const bool&>();
}
void Config::setBool(const std::string &key, bool v) {
	this->json[key] = v;
}

int Config::getInt(const std::string &key) {
	if (!this->json.contains(key)) {
		return 0;
	}

	return this->json.at(key).get_ref<const int64_t&>();
}
void Config::setInt(const std::string &key, int v) {
	this->json[key] = v;
}

std::string Config::getString(const std::string &key) {
	if (!this->json.contains(key)) {
		return "";
	}

	return this->json.at(key).get_ref<const std::string&>();
}
void Config::setString(const std::string &key, const std::string &v) {
	this->json[key] = v;
}