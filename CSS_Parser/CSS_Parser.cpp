#define _CRT_SECURE_NO_WARNINGS

#define READ_SELECTOR 0
#define READ_ATTRIBUTE 1
#define READ_VALUE 2
#define READ_COMMAND_INI 3
#define READ_COMMAND_END 4
#define READ_COMMAND 5

#define SIM_COMMAND 0
#define I_COMMAND 1
#define NZ_COMMAND 2
#define PARAMETER_2 3
#define PARAMETER_3 4

#include <iostream>
#include <string.h>
#include<stdexcept>
using namespace std;


const int T = 8;

class String
{
    char* charArr;
public:

    String()
    {
        charArr = NULL;
    }

    explicit String(const char* charArr)
    {
        this->charArr = new char[strlen(charArr) + 1];
        strcpy_s(this->charArr, strlen(charArr) + 1, charArr);
    }

    String(const String& other)
    {
        if (other.charArr != nullptr)
        {
            size_t len = strlen(other.charArr);
            this->charArr = new char[len + 1];
            strcpy_s(this->charArr, len + 1, other.charArr);
        }
        else
        {
            this->charArr = nullptr;
        }
    }

    size_t length() const
    {
        if (charArr)
            return strlen(charArr);
        else
            return 0;
    }

    const char* data() const
    {
        return charArr;
    }
    ;
    String& operator=(const char* str)
    {
        char* newCharArr = new char[strlen(str) + 1];
        if (newCharArr)
        {
            delete[] charArr;
            charArr = newCharArr;
            strcpy_s(charArr, strlen(str) + 1, str);
        }
        else
        {
            throw std::bad_alloc();
        }
        return *this;
    }

    String& operator=(const String& other)
    {
        if (this != &other)
        {
            delete[] this->charArr;
            if (other.charArr != nullptr)
            {
                size_t len = strlen(other.charArr);
                this->charArr = new char[len + 1];
                strcpy_s(this->charArr, len + 1, other.charArr);
            }
            else
            {
                this->charArr = nullptr;
            }
        }
        return *this;
    }

    bool operator==(const String& other) const
    {
        if (strcmp(this->charArr, other.charArr) == 0)
        {
            return true;
        }
        return false;
    }

    bool operator!=(const String& other) const
    {
        return !(*this == other);
    }

    String operator+(const String& other)
    {
        if (this->charArr == NULL)
        {
            *this = other;
            return *this;
        }
        else
        {
            char* temp;
            temp = new char[strlen(this->charArr) + strlen(other.charArr) + 1];
            strcpy_s(temp, strlen(this->charArr) + 1, this->charArr);
            strcat(temp, other.charArr);
            String String(temp);
            delete[] temp;
            return String;
        }
    }

    void push_back(char c)
    {
        size_t oldLength = length();
        char* newCharArr = new char[oldLength + 2];
        if (oldLength > 0) {
            strcpy_s(newCharArr, oldLength + 1, charArr);
        }
        newCharArr[oldLength] = c;
        newCharArr[oldLength + 1] = '\0';
        delete[] charArr;
        charArr = newCharArr;
    }

    void removeEmpty()
    {
        for (int i = length() - 1; i >= 0; i--)
        {
            if (charArr[i] == ' ')
                charArr[i] = '\0';
            else
                return;
        }
    }

    virtual ~String()
    {
        if (charArr != NULL)
        {
            delete[] charArr;
        }
    }

    friend ostream& operator<<(ostream& os, const String& charArr)
    {
        if (charArr.charArr) {
            os << charArr.charArr;
        }
        return os;
    }

    friend istream& operator >> (istream& is, String& charArr)
    {
        char buffer[1024];
        is >> buffer;


        if (charArr.charArr) {
            delete charArr.charArr;
            charArr.charArr = NULL;
        }

        charArr.charArr = new char[strlen(buffer) + 1];
        strcpy_s(charArr.charArr, strlen(buffer) + 1, buffer);

        return is;
    }
};

struct Attribute
{
    String name;
    String value;
    Attribute* next;
};

struct Selector
{
    String name;
    Selector* next;
};

/*Section
* Selectors
* {
*   Attribute1: value1
*   Attribute2: value2
* }
*/

class Section
{
public:

    Selector* selectorList;
    Attribute* attributeList;

    Section()
    {
        selectorList = nullptr;
        attributeList = nullptr;
    }

    void addSelector(String _name)
    {
        if (selectorList == nullptr)
        {
            selectorList = new Selector;
            selectorList->name = _name;
            selectorList->next = nullptr;
        }
        else
        {
            removeSelectors(_name);
            Selector* currentSelector = selectorList;
            while (currentSelector->next != nullptr)
            {
                currentSelector = currentSelector->next;
            }
            currentSelector->next = new Selector;
            currentSelector->next->name = _name;
            currentSelector->next->next = nullptr;
        }
    }

    void addAttribute(String _name, String _value)
    {

        removeAttributes(_name);
        if (attributeList == nullptr)
        {
            attributeList = new Attribute;
            attributeList->name = _name;
            attributeList->value = _value;
            attributeList->next = nullptr;
        }
        else
        {
            Attribute* currentAttribute = attributeList;
            while (currentAttribute->next != nullptr)
            {
                currentAttribute = currentAttribute->next;
            }
            currentAttribute->next = new Attribute;
            currentAttribute->next->name = _name;
            currentAttribute->next->value = _value;
            currentAttribute->next->next = nullptr;
        }
    }

    void removeAttributes(String _name)
    {
        if (attributeList != nullptr)
        {
            Attribute* currentAttribute = attributeList;
            if (currentAttribute->name == _name)
            {
                if (currentAttribute->next != nullptr)
                    attributeList = currentAttribute->next;
                else
                    attributeList = nullptr;
                currentAttribute = nullptr;
            }
            if (currentAttribute != nullptr)
            {
                while (currentAttribute != nullptr && currentAttribute->next != nullptr)
                {
                    if (currentAttribute->next->name == _name)
                    {
                        currentAttribute->next = currentAttribute->next->next;
                    }
                    currentAttribute = currentAttribute->next;
                }
            }
        }
    }

    void printAttributes() const
    {
        cout << "{\n";
        if (attributeList != nullptr)
        {
            Attribute* currentAttribute = attributeList;
            while (currentAttribute->next != nullptr)
            {
                cout << "\t" << currentAttribute->name << ": " << currentAttribute->value << ";" << endl;
                currentAttribute = currentAttribute->next;
            }
            cout << "\t" << currentAttribute->name << ": " << currentAttribute->value << ";" << endl;
        }
        cout << "}";
    }

    void printSelectors() const
    {
        if (selectorList != nullptr)
        {
            Selector* currentSelector = selectorList;
            while (currentSelector->next != nullptr)
            {
                cout << currentSelector->name << ", ";
                currentSelector = currentSelector->next;
            }
            cout << currentSelector->name << endl;
        }
    }

    void printSection()
    {
        printSelectors();
        printAttributes();
    }

    void removeSelectors(String _name)
    {
        if (selectorList->name.data() != NULL && _name.data() != NULL)
        {
            Selector* currentSelector = selectorList;
            //obs³uga pierwszego selektora
            if (currentSelector->name == _name)
                selectorList = currentSelector->next;
            while (currentSelector->next != nullptr)
            {
                if (currentSelector->next->name.data() != NULL && currentSelector->next->name == _name)
                {
                    currentSelector->next = currentSelector->next->next;
                }
                currentSelector = currentSelector->next;
            }
        }
    }

    int countSelectors() const
    {
        int counter = 0;
        if (selectorList != nullptr)
        {
            Selector* currentSelector = selectorList;
            while (currentSelector->next != nullptr)
            {
                counter++;
                currentSelector = currentSelector->next;
            }
            if (currentSelector->name.data() != NULL)
                counter++;
        }
        return counter;
    }

    int countSelectorsNamed(String _name) const
    {
        int counter = 0;
        if (selectorList != nullptr)
        {
            Selector* currentSelector = selectorList;
            while (currentSelector != nullptr)
            {
                if (currentSelector->name.data() != NULL && currentSelector->name == _name)
                    counter++;
                currentSelector = currentSelector->next;
            }
        }
        return counter;
    }

    int countAttributes() const
    {
        int counter = 0;
        if (selectorList != nullptr)
        {
            Attribute* currentAttribute = attributeList;
            while (currentAttribute->next != nullptr)
            {
                counter++;
                currentAttribute = currentAttribute->next;
            }
            counter++;
        }
        return counter;
    }

    int removeAttribute(String _name)
    {
        if (attributeList != nullptr)
        {
            Attribute* currentAttribute = attributeList;
            if (currentAttribute->name == _name)
            {
                if (currentAttribute->next != nullptr)
                {
                    attributeList = currentAttribute->next;
                    return 1;
                }
                else
                    return -1;
            }
            while (currentAttribute->next != nullptr)
            {
                if (currentAttribute->next->name == _name)
                {
                    currentAttribute->next = currentAttribute->next->next;
                    return 1;
                }
                currentAttribute = currentAttribute->next;
            }
        }
        return 0;
    }

    bool ifThereIsSelector(String _name) const
    {
        if (selectorList != nullptr)
        {
            Selector* currentSelector = selectorList;
            while (currentSelector != nullptr)
            {
                if (currentSelector->name.data() != NULL && currentSelector->name == _name)
                    return true;
                currentSelector = currentSelector->next;
            }
            return false;
        }
        return false;
    }

    bool ifThereIsAttribute(String _name) const
    {
        if (attributeList != nullptr)
        {
            Attribute* currentAttribute = attributeList;
            while (currentAttribute != nullptr)
            {
                if (currentAttribute->name == _name)
                    return true;
                currentAttribute = currentAttribute->next;
            }
            return false;
        }
        return false;
    }

    Selector* getSelector(int selectorNo)
    {
        if (selectorList != nullptr && selectorList->name.data() != NULL)
        {
            Selector* currentSelector = selectorList;
            for (int i = 0; i < selectorNo; i++)
            {
                if (currentSelector->next != nullptr)
                    currentSelector = currentSelector->next;
                else
                    return nullptr;
            }
            return currentSelector;
        }
        else
            return nullptr;
    }

    Attribute* getAttribute(String _name)
    {
        if (attributeList != nullptr)
        {
            Attribute* currentAttribute = attributeList;
            while (currentAttribute != nullptr)
            {
                if (currentAttribute->name == _name)
                    return currentAttribute;
                currentAttribute = currentAttribute->next;
            }
        }
        return nullptr;
    }

    ~Section() {}
};

class Block
{
private:

    Section** sectionList;

public:

    Block* previous;
    Block* next;

    Block()
    {
        previous = nullptr;
        next = nullptr;
        sectionList = new Section * [T];
        for (int i = 0; i < T; i++)
            sectionList[i] = nullptr;
    }

    Block* addSection(Section* _section)
    {
        bool done = false;
        int i = 0;
        while (!done && i < T)
        {
            if (sectionList[i] == nullptr)
            {
                sectionList[i] = _section;
                done = true;
                return this;
            }
            i++;
        }
        if (i >= T && !done)
        {
            Block* temp = new Block;
            temp->addSection(_section);
            addBlock(temp);
            return temp;
        }
        return nullptr;
    }

    bool removeSection(int _index)
    {
        if (_index < T && sectionList[_index] != nullptr)
        {
            sectionList[_index] = nullptr;
            for (int i = _index + 1; i < T; i++)
            {
                if (sectionList[i] != nullptr && sectionList[i - 1] == nullptr)
                {
                    sectionList[i - 1] = sectionList[i];
                    sectionList[i] = nullptr;
                }
            }
            if (next != nullptr)
            {
                sectionList[T - 1] = next->sectionList[0];
                next->removeSection(0);
            }
            return true;
        }
        else
            return false;
    }

    bool removeAttribute(int _index, String _name)
    {
        if (sectionList[_index] != nullptr)
        {
            int temp = sectionList[_index]->removeAttribute(_name);
            if (temp == 1)
                return true;
            else if (temp == -1)
            {
                removeSection(_index);
                return true;
            }
            else
                return false;
        }
        else
            return false;
    }

    void printBlock()
    {
        for (int i = 0; i < T; i++)
        {
            if (sectionList[i] != nullptr)
            {
                sectionList[i]->printSection();
                cout << endl;
            }
        }
    }

    void removeSelectors(String _name)
    {
        for (int i = 0; i < T; i++)
            if (sectionList[i] != nullptr)
            {
                sectionList[i]->removeSelectors(_name);
                if (sectionList[i]->selectorList == nullptr)
                    removeSection(i);
            }
    }

    void debugPrintBlock()
    {
        for (int i = 0; i < T; i++)
        {
            if (sectionList[i] != nullptr)
            {
                cout << "Section nr. " << i + 1 << " :\n";
                sectionList[i]->printSection();
                cout << endl;
            }
        }
    }

    int countSections() const
    {
        int counter = 0;
        for (int i = 0; i < T; i++)
            if (sectionList != NULL && sectionList[i] != nullptr)
            {
                counter++;
            }
        return counter;
    }

    int countSelectorsInSection(int _index)
    {
        if (sectionList[_index] != nullptr)
            return sectionList[_index]->countSelectors();
        else
            return -1;
    }

    int countAttributesInSection(int _index)
    {
        if (sectionList[_index] != nullptr)
            return sectionList[_index]->countAttributes();
        else
            return -1;
    }

    int countSelectors(String _name)
    {
        int counter = 0;
        for (int i = 0; i < T; i++)
            if (sectionList[i] != nullptr)
                counter += sectionList[i]->countSelectorsNamed(_name);
        return counter;
    }

    int countAttributesNamed(String _name)
    {
        int counter = 0;
        for (int i = 0; i < T; i++)
            if (sectionList[i] != nullptr)
                if (sectionList[i]->ifThereIsAttribute(_name))
                    counter++;
        return counter;
    }

    Section* getSectionWithSelector(String _name)
    {
        Section* temp = nullptr;
        for (int i = 0; i < T; i++)
            if (sectionList[i] != nullptr && sectionList[i]->ifThereIsSelector(_name))
                temp = sectionList[i];
        return temp;
    }

    Selector* getSelectorFromSection(int sectionNo, int selectorNo)
    {
        if (sectionList[sectionNo] != nullptr)
            return sectionList[sectionNo]->getSelector(selectorNo);
        else
            return nullptr;
    }

    Attribute* getAttributeFromSection(int sectionNo, String name)
    {
        if (sectionList[sectionNo] != nullptr)
            return sectionList[sectionNo]->getAttribute(name);
        else
            return nullptr;
    }

    void addBlock(Block* _block)
    {
        Block* currentBlock = this;
        while (currentBlock->next != nullptr)
        {
            currentBlock = currentBlock->next;
        }
        currentBlock->next = _block;
        _block->previous = this;
    }

    ~Block() {}
};

class BlockList
{
private:

    Block* first;
    Block* last;

public:

    BlockList()
    {
        first = new Block;
        last = first;
    }

    void addBlock(Block* _block)
    {
        if (last == nullptr)
        {
            first = _block;
            last = _block;
        }
        else
        {
            last->addBlock(_block);
            last = last->next;
        }
    }

    void printBlocks() const
    {
        Block* currentBlock = first;
        while (currentBlock != nullptr)
        {
            currentBlock->printBlock();
            currentBlock = currentBlock->next;
        }
    }

    void removeSelectors(String _name) const
    {
        Block* currentBlock = first;
        while (currentBlock != nullptr)
        {
            currentBlock->removeSelectors(_name);
            currentBlock = currentBlock->next;
        }
    }

    void addSection(Section* _section)
    {
        /*
        Selector* currentSelector = _section->selectorList;
        while (currentSelector != nullptr)
        {
            removeSelectors(currentSelector->name);
            currentSelector = currentSelector->next;
        }
        */
        last = last->addSection(_section);
    }

    void debugPrintBlocks()
    {
        Block* currentBlock = first;
        int i = 1;
        while (currentBlock != nullptr)
        {
            cout << "Block nr. " << i << " :\n";
            currentBlock->debugPrintBlock();
            currentBlock = currentBlock->next;
            i++;
        }
    }

    bool removeSection(int sectionNo)
    {
        int index = (sectionNo - 1) % T;
        int blockNo = (sectionNo - 1) / T;
        Block* currentBlock = first;
        bool temp;
        for (int i = 0; i < blockNo; i++)
        {
            if (currentBlock->next != nullptr)
                currentBlock = currentBlock->next;
            else
                return false;
        }
        temp = currentBlock->removeSection(index);
        if (last->countSections() == 0 && last->previous != nullptr)
        {
            last = last->previous;
            last->next = nullptr;
        }
        if (first->countSections() == 0)
        {
            first = new Block;
            last = first;
        }
        return temp;
    }

    bool removeAttribute(int sectionNo, String name) const
    {
        int index = (sectionNo - 1) % T;
        int blockNo = (sectionNo - 1) / T;
        Block* currentBlock = first;
        for (int i = 0; i < blockNo; i++)
        {
            if (currentBlock->next != nullptr)
                currentBlock = currentBlock->next;
            else
                return false;
        }
        return currentBlock->removeAttribute(index, name);
    }

    int countSections() const
    {
        int counter = 0;
        if (first != nullptr)
        {
            Block* currentBlock = first;
            while (currentBlock->next != nullptr)
            {
                counter += currentBlock->countSections();
                currentBlock = currentBlock->next;
            }
            counter += currentBlock->countSections();
        }
        return counter;
    }

    int countSelectorsInSection(int _index) const
    {
        int sectionNo = (_index - 1) % T;
        int blockNo = (_index - 1) / T;
        Block* currentBlock = first;
        for (int i = 0; i < blockNo; i++)
        {
            if (currentBlock->next != nullptr)
                currentBlock = currentBlock->next;
            else
                return -1;
        }
        return currentBlock->countSelectorsInSection(sectionNo);
    }

    int countAttributesInSection(int _index) const
    {
        int sectionNo = (_index - 1) % T;
        int blockNo = (_index - 1) / T;
        Block* currentBlock = first;
        for (int i = 0; i < blockNo; i++)
        {
            if (currentBlock->next != nullptr)
                currentBlock = currentBlock->next;
            else
                return -1;
        }
        return currentBlock->countAttributesInSection(sectionNo);
    }

    int countSelectors(String _name) const
    {
        int counter = 0;
        Block* currentBlock = first;
        while (currentBlock != nullptr)
        {
            counter += currentBlock->countSelectors(_name);
            currentBlock = currentBlock->next;
        }
        return counter;
    }

    int countAttributes(String _name) const
    {
        int counter = 0;
        Block* currentBlock = first;
        while (currentBlock != nullptr)
        {
            counter += currentBlock->countAttributesNamed(_name);
            currentBlock = currentBlock->next;
        }
        return counter;
    }

    Selector* getSelectorFromSection(int sectionNo, int selectorNo)
    {
        int section = (sectionNo - 1) % T;
        int blockNo = (sectionNo - 1) / T;
        Block* currentBlock = first;
        for (int i = 0; i < blockNo; i++)
        {
            if (currentBlock->next != nullptr)
                currentBlock = currentBlock->next;
            else
                return nullptr;
        }
        return currentBlock->getSelectorFromSection(section, selectorNo - 1);
    }

    Attribute* getAttributeFromSection(int sectionNo, String name)
    {
        int section = (sectionNo - 1) % T;
        int blockNo = (sectionNo - 1) / T;
        Block* currentBlock = first;
        for (int i = 0; i < blockNo; i++)
        {
            if (currentBlock->next != nullptr)
                currentBlock = currentBlock->next;
            else
                return nullptr;
        }
        return currentBlock->getAttributeFromSection(section, name);
    }

    Attribute* getAttribteOfSelector(String selector, String attribute)
    {
        Block* currentBlock = first;
        Section* temp = nullptr;
        Section* tempSection = nullptr;
        while (currentBlock != nullptr)
        {
            temp = currentBlock->getSectionWithSelector(selector);
            if (temp != nullptr)
                tempSection = temp;
            currentBlock = currentBlock->next;
        }
        if (tempSection != nullptr)
            return tempSection->getAttribute(attribute);
        else
            return nullptr;
    }

    ~BlockList()
    {
        delete first;
        delete last;
    }
};

struct Command
{
    char type;
    int i, j = NULL;
    char parameter2 = NULL;
    bool hasStr = false;
    String n, n2;
};

void printCommand(Command* command)
{
    if (command->type == 'i')
        cout << command->i;
    else
        cout << command->n;
    cout << "," << command->parameter2 << ",";
    if (command->hasStr)
        cout << command->n2;
    else
        cout << command->j;
}

void doCommand(Command* command, BlockList* blockList)
{
    if (command->type != 'i' && (command->parameter2 == 'S' || command->parameter2 == 'E'))
        command->type = 'z';

    if (command->type == 'i')
    {
        if (command->parameter2 == 'S')
        {
            //i,S,?
            if (command->hasStr && command->n2.data()[0] == '?')
            {

                int temp = blockList->countSelectorsInSection(command->i);
                if (temp >= 0)
                {
                    printCommand(command);
                    cout << " == " << temp << endl;
                }
            }
            //i,S,j
            else
            {
                Selector* temp = blockList->getSelectorFromSection(command->i, command->j);
                if (temp != nullptr)
                {
                    printCommand(command);
                    cout << " == " << temp->name << endl;
                }
            }
        }
        else if (command->parameter2 == 'A')
        {
            //i,A,?
            if (command->hasStr && command->n2.data()[0] == '?')
            {

                int temp = blockList->countAttributesInSection(command->i);
                if (temp >= 0)
                {
                    printCommand(command);
                    cout << " == " << temp << endl;
                }
            }
            //i,A,n
            else
            {
                Attribute* temp = blockList->getAttributeFromSection(command->i, command->n2);
                if (temp != nullptr)
                {
                    printCommand(command);
                    cout << " == " << temp->value << endl;
                }
            }
        }
        else if (command->parameter2 == 'D')
        {
            //i,D,*
            if (command->hasStr && command->n2.data()[0] == '*')
            {
                if (blockList->removeSection(command->i))
                {
                    printCommand(command);
                    cout << " == deleted" << endl;
                }
            }
            //i,D,n
            else
            {
                if (blockList->removeAttribute(command->i, command->n2))
                {
                    printCommand(command);
                    cout << " == deleted" << endl;
                }
            }
        }
    }
    else if (command->type == 'z')
    {
        //z,S,?
        if (command->parameter2 == 'S')
        {
            printCommand(command);
            cout << " == " << blockList->countSelectors(command->n) << endl;
        }
        //z,E,n
        else
        {
            Attribute* temp = blockList->getAttribteOfSelector(command->n, command->n2);
            if (temp != nullptr)
            {
                printCommand(command);
                cout << " == " << temp->value << endl;
            }
        }
    }
    //n,A,?
    else if (command->type == 'n' && command->parameter2 == 'A')
    {
        printCommand(command);
        cout << " == " << blockList->countAttributes(command->n) << endl;
    }
}

void readInput(BlockList* blockList)
{
    int mode = READ_SELECTOR,
        submode = SIM_COMMAND,
        counter = 0,
        counter2 = 0;
    bool done = false;
    Section* currSection = nullptr;
    Command* currentCommand = nullptr;
    char c;
    /*
    * MODE:
    *
    * CSS READING
    * 0 - selector
    * 1 - attribute
    * 2 - value
    *
    * COMMANDS READING
    * W.I.P.
    * 3 - command
    */
    while (true)
    {
        String str, str2;
        bool wasFirst = false;
        while (!done)
        {
            c = getchar();
            if (c == EOF)
                return;
            //if (c == '`')
            //    blockList->printBlocks();
            if (c == '`')
                blockList->debugPrintBlocks();
            //CSS handling
            if (mode == READ_SELECTOR)
            {
                if (c == '?')
                    mode = READ_COMMAND_INI;
                if (currSection == nullptr)
                    currSection = new Section;
                if (c != '\n' && c != '\t' && c != '?' && c != char(27) && c != '\r' && c != '`')
                {
                    if (c == ',')
                    {
                        str.removeEmpty();
                        currSection->addSelector(str);
                        done = true;
                    }
                    else if (c == '{')
                    {
                        str.removeEmpty();
                        currSection->addSelector(str);
                        mode = READ_ATTRIBUTE;
                        wasFirst = false;
                        done = true;
                    }
                    else if (c == ' ' && wasFirst)
                    {
                        str.push_back(c);
                    }
                    else if (c != ' ')
                    {
                        wasFirst = true;
                        str.push_back(c);
                    }
                }
            }
            if (mode == READ_ATTRIBUTE)
            {
                if (currSection == nullptr)
                    currSection = new Section;
                if (c != '\n' && c != '\t' && c != char(27) && c != '\r' && c != '`')
                {
                    if (c == ':')
                    {
                        wasFirst = false;
                        mode = READ_VALUE;
                    }
                    else if (c == '}')
                    {
                        blockList->addSection(currSection);
                        currSection = nullptr;
                        mode = READ_SELECTOR;
                        done = true;
                    }
                    else if (c == ' ' && wasFirst)
                    {
                        str.push_back(c);
                    }
                    else if (c != ' ')
                    {
                        wasFirst = true;
                        str.push_back(c);
                    }
                }
            }
            if (mode == READ_VALUE)
            {
                if (c != '\n' && c != '\t' && c != ':' && c != '!' && c != 27 && c != '\r' && c != '`')
                {
                    if (c == ';')
                    {
                        str.removeEmpty();
                        str2.removeEmpty();
                        currSection->addAttribute(str, str2);
                        mode = READ_ATTRIBUTE;
                        done = true;
                    }
                    else if (c == '}')
                    {
                        str.removeEmpty();
                        str2.removeEmpty();
                        currSection->addAttribute(str, str2);
                        blockList->addSection(currSection);
                        currSection = nullptr;
                        mode = READ_SELECTOR;
                        done = true;
                    }
                    else if (c == ' ' && wasFirst)
                    {
                        str2.push_back(c);
                    }
                    else if (c != ' ')
                    {
                        wasFirst = true;
                        str2.push_back(c);
                    }
                }
            }
            //command handling
            if (mode == READ_COMMAND_INI)
            {
                if (counter >= 4)
                {
                    counter2 = 0;
                    mode = READ_COMMAND;
                }
                if (c == '?')
                    counter++;
            }
            if (mode == READ_COMMAND_END)
            {
                if (counter2 >= 3)
                {
                    counter = 0;
                    mode = READ_SELECTOR;
                }
                if (c == '*')
                    counter2++;
            }
            if (mode == READ_COMMAND)
            {
                if (c == '*' && submode == SIM_COMMAND)
                    mode = READ_COMMAND_END;
                else if (c == '?' && submode == SIM_COMMAND)
                    cout << "? == " << blockList->countSections() << endl;
                else
                {
                    if (currentCommand == nullptr)
                        currentCommand = new Command;
                    if (int(c) > 47 && int(c) < 58 && submode == SIM_COMMAND)
                        submode = I_COMMAND;
                    else if (submode == SIM_COMMAND && c != ' ' && c != '\n' && c != '\t' && c != '*' && c != char(27) && c != '\r' && c != '`')
                        submode = NZ_COMMAND;
                    if (submode == I_COMMAND)
                    {
                        currentCommand->type = 'i';
                        if (c == ',')
                        {
                            currentCommand->i = atoi(str.data());
                            submode = PARAMETER_2;
                            done = true;
                        }
                        if (int(c) > 47 && int(c) < 58)
                        {
                            str.push_back(c);
                        }
                    }
                    else if (submode == NZ_COMMAND)
                    {
                        currentCommand->type = 'n';
                        if (c == ',')
                        {
                            currentCommand->n = str;
                            submode = PARAMETER_2;
                            done = true;
                        }
                        else
                            str.push_back(c);
                    }
                    else if (submode == PARAMETER_2)
                    {
                        if (c == ',')
                        {
                            submode = PARAMETER_3;
                            done = true;
                        }
                        else
                            currentCommand->parameter2 = c;
                    }
                    else if (submode == PARAMETER_3)
                    {
                        if (c == ' ' || c == '\n' || c == '\t' || c == '\0' || c == '\r')
                        {
                            //third parameter being int
                            if (str.data() != NULL && int(str.data()[0]) > 47 && int(str.data()[0]) < 58)
                            {
                                currentCommand->j = atoi(str.data());
                                currentCommand->hasStr = false;
                            }
                            //third parameter being char[]
                            else if (str.data() != NULL)
                            {
                                currentCommand->n2 = str;
                                currentCommand->hasStr = true;
                            }
                            doCommand(currentCommand, blockList);
                            submode = SIM_COMMAND;
                            done = true;
                        }
                        //third parameter being ?
                        else if (c == '?')
                        {
                            str.push_back(c);
                            currentCommand->n2 = str;
                            currentCommand->hasStr = true;
                            doCommand(currentCommand, blockList);
                            submode = SIM_COMMAND;
                            done = true;
                        }
                        else
                        {
                            str.push_back(c);
                        }
                    }
                }
            }
        }
        done = false;
    }
}

int main()
{
    BlockList* blockList = new BlockList;
    readInput(blockList);
}