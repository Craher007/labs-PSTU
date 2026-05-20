#ifndef DATABASE_H
#define DATABASE_H

#include "Models.h"
#include <string>
#include <vector>

using namespace std;

class Database {
private:
    vector<Item> items;
    string fileName;

    vector<string> split(const string& line, char delimiter) const;

public:
    Database(const string& fileName);

    bool load();
    bool save() const;

    Item* findItem(const string& name);
    const Item* findItem(const string& name) const;

    bool addOrUpdateItem(const Item& item);
    bool removeItem(const string& name);
    bool renameItem(const string& oldName, const string& newName);
    void clear();

    const vector<Item>& getItems() const;
    string getFileName() const;
};

#endif
