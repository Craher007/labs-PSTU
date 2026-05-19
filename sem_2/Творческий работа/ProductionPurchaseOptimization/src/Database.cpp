#include "Database.h"
#include <fstream>
#include <sstream>

using namespace std;

static string trimText(const string& value) {
    // Убираем случайные пробелы по краям, чтобы "Брус" и " Брус " не стали разными записями.
    size_t start = value.find_first_not_of(" \t\r\n");
    if (start == string::npos) {
        return "";
    }

    size_t end = value.find_last_not_of(" \t\r\n");
    return value.substr(start, end - start + 1);
}

static Item prepareItemForSave(const Item& source) {
    // Перед сохранением очищаем запись от пустых названий и компонентов с нулевым количеством.
    // Это защищает файл базы от случайных пустых строк из интерфейса.
    Item item = source;
    item.name = trimText(item.name);
    item.unit = trimText(item.unit);

    vector<Component> components;
    for (Component component : item.components) {
        component.name = trimText(component.name);
        component.unit = trimText(component.unit);

        if (!component.name.empty() && component.quantity > 0) {
            components.push_back(component);
        }
    }

    item.components = components;
    return item;
}

Database::Database(const string& fileName) {
    this->fileName = fileName;
}

vector<string> Database::split(const string& line, char delimiter) const {
    vector<string> parts;
    string part;
    stringstream stream(line);

    while (getline(stream, part, delimiter)) {
        parts.push_back(part);
    }

    return parts;
}

bool Database::load() {
    // Формат базы простой текстовый:
    // ITEM|название|единица|тип|дни|параллельно/партия
    // COMP|родитель|компонент|количество|единица
    items.clear();

    ifstream file(fileName);
    if (!file.is_open()) {
        return false;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        vector<string> parts = split(line, '|');

        try {
            if (parts.size() >= 6 && parts[0] == "ITEM") {
                Item item;
                item.name = trimText(parts[1]);
                item.unit = trimText(parts[2]);
                item.type = static_cast<ItemType>(stoi(parts[3]));
                item.days = stoi(parts[4]);
                item.capacity = stod(parts[5]);

                if (!item.name.empty()) {
                    items.push_back(item);
                }
            } else if (parts.size() >= 5 && parts[0] == "COMP") {
                string parentName = trimText(parts[1]);
                string componentName = trimText(parts[2]);

                if (parentName.empty() || componentName.empty()) {
                    continue;
                }

                Item* parent = findItem(parentName);
                if (parent != nullptr) {
                    Component component;
                    component.name = componentName;
                    component.quantity = stod(parts[3]);
                    component.unit = trimText(parts[4]);

                    if (component.quantity > 0) {
                        parent->components.push_back(component);
                    }
                }
            }
        } catch (...) {
            // Если строка в базе испорчена, пропускаем ее, чтобы программа не завершалась аварийно.
        }
    }

    return true;
}

bool Database::save() const {
    // Сначала записываем саму запись ITEM, затем сразу ее компоненты COMP.
    // Такой формат легко посмотреть и исправить вручную, если понадобится.
    ofstream file(fileName);
    if (!file.is_open()) {
        return false;
    }

    for (const Item& item : items) {
        Item prepared = prepareItemForSave(item);

        if (prepared.name.empty()) {
            continue;
        }

        file << "ITEM|" << prepared.name << "|" << prepared.unit << "|"
             << prepared.type << "|" << prepared.days << "|" << prepared.capacity << "\n";

        for (const Component& component : prepared.components) {
            file << "COMP|" << prepared.name << "|" << component.name << "|"
                 << component.quantity << "|" << component.unit << "\n";
        }
    }

    return true;
}

Item* Database::findItem(const string& name) {
    string cleanName = trimText(name);

    for (Item& item : items) {
        if (trimText(item.name) == cleanName) {
            return &item;
        }
    }

    return nullptr;
}

const Item* Database::findItem(const string& name) const {
    string cleanName = trimText(name);

    for (const Item& item : items) {
        if (trimText(item.name) == cleanName) {
            return &item;
        }
    }

    return nullptr;
}

bool Database::addOrUpdateItem(const Item& item) {
    // Если запись уже есть - заменяем ее, если нет - добавляем новую.
    // Возвращаемое значение показывает, была ли запись добавлена впервые.
    Item prepared = prepareItemForSave(item);
    if (prepared.name.empty()) {
        return false;
    }

    Item* existing = findItem(prepared.name);

    if (existing != nullptr) {
        *existing = prepared;
        return false;
    }

    items.push_back(prepared);
    return true;
}

bool Database::removeItem(const string& name) {
    bool removed = false;
    string cleanName = trimText(name);

    for (size_t i = 0; i < items.size(); i++) {
        if (trimText(items[i].name) == cleanName) {
            items.erase(items.begin() + i);
            removed = true;
            break;
        }
    }

    // Удаляем ссылки на эту запись из составов других изделий.
    for (Item& item : items) {
        for (size_t i = 0; i < item.components.size();) {
            if (trimText(item.components[i].name) == cleanName) {
                item.components.erase(item.components.begin() + i);
            } else {
                i++;
            }
        }
    }

    return removed;
}

bool Database::renameItem(const string& oldName, const string& newName) {
    string cleanOldName = trimText(oldName);
    string cleanNewName = trimText(newName);

    if (cleanNewName.empty() || findItem(cleanNewName) != nullptr) {
        return false;
    }

    Item* item = findItem(cleanOldName);
    if (item == nullptr) {
        return false;
    }

    item->name = cleanNewName;

    // Обновляем название во всех составах изделий.
    for (Item& parent : items) {
        for (Component& component : parent.components) {
            if (trimText(component.name) == cleanOldName) {
                component.name = cleanNewName;
            }
        }
    }

    return true;
}

void Database::clear() {
    // Полная очистка нужна для кнопки "Удалить всю базу" в окне Qt.
    items.clear();
}

const vector<Item>& Database::getItems() const {
    return items;
}

string Database::getFileName() const {
    return fileName;
}
