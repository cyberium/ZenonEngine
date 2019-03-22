#pragma once

class XMLNode
{
public:
	XMLNode(cstring _name, XMLNode* _parent);
	void DeleteChilds();
	void DeleteAll();

	void SetAsDataNode();

	std::string GetName() { return name; };
	XMLNode* GetParent() { return parent; };
	std::vector<XMLNode*>& GetChilds() { return childs; }
	std::map<std::string, std::string>& GetData() { return data; };
	bool IsKeyExists(cstring _keyName) const { return data.find(_keyName) != data.end(); }
	std::string GetKeyValue(std::string _keyName)
	{
		if (!IsKeyExists(_keyName))
			return "";
		return data[_keyName];
	}
	bool IsDataNode() const { return isDataNode; }

	void AddChild(XMLNode* _node);
	void AddData(cstring _keyName, cstring _data, bool append = false);

private:
	std::string name;
	XMLNode* parent;
	std::vector<XMLNode*> childs;
	std::map<std::string, std::string> data;

	bool isDataNode;
};

class XMLFile
{
public:
	 bool Open(cstring _filename);
	 void Destroy();

	 void Print(XMLNode* _startNode, int _level = 0) const;

	 XMLNode* GetRootNode() const { return rootNode; };

private:
	bool ProcessLine(std::string& _line);
	bool ProcessTag(std::string& _tag);
	bool ProcessData(std::string& _data);
	void DeleteDataNodes(XMLNode* _startNode);

private:
	std::string filename;
	XMLNode* rootNode;
};
