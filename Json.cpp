#include "json.h"

namespace sg
{
    inline bool isdigit(char c) { return c >= '0' && c <= '9'; }
    inline bool ishexdigit(char c) { return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z'); }

    inline void skipwhitespace(const string &json_text, int i)
    {
        while (json_text[i] == ' ')
            ++i;
    }

    void json::parsebool(const string &key, const string &json_text, int i, const char *context)
    {
        int offset = 0;
        for (; context[offset]; ++offset)
        {
            if (json_text[i + offset] != context[offset])
            {
                cerr << "error:parsebool():invalid value." << endl;
                return;
            }
        }
        members.emplace(make_pair(key, json_value(offset == 4)));
    }
    void json::parsenull(const string &key, const string &json_text, int i, const char *context)
    {
        int offset = 0;
        for (; context[offset]; ++offset)
        {
            if (json_text[i + offset] != context[offset])
            {
                cerr << "error:parsenull():invalid value." << endl;
                return;
            }
        }
        members.emplace(make_pair(key, json_value()));
    }
    void json::parsenum(const string &key, const string &json_text, int i)
    {
        int start = i;
        if (json_text[i] == '-')
            ++i;
        if (!isdigit(json_text[i]))
        {
            cerr << "error:parsenum():invalid value." << endl;
            return;
        }
        while (isdigit(json_text[i]))
            ++i;

        bool isdoube = json_text[i] == '.';
        if (isdoube)
        {
            ++i;
            if (!isdigit(json_text[i]))
            {
                cerr << "error:parsenum():invalid value." << endl;
                return;
            }
            while (isdigit(json_text[i]))
                ++i;
        }
        if (json_text[i] == 'e' || json_text[i] == 'E')
        {
            ++i;
            if (!isdigit(json_text[i]))
            {
                cerr << "error:parsenum():invalid value." << endl;
                return;
            }
            if (json_text[i] == '+' || json_text[i] == '-')
                ++i;
            while (isdigit(json_text[i]))
                ++i;
        }
        string tmp(json_text, start, i - start);
        members.emplace(make_pair(key, json_value(stod(tmp)))); // note:stod()只接受左值和左值引用,不可stod(string());后来看cppreference发现貌似只需要给stod()第二个参数起始点，就可以让它自己解析。
    }
    void json::parsestring(const string &key, const string &json_text, int i)
    {
        if (json_text[i] != '"')
        {
            cerr << "error:Expect left quotation." << endl;
            return;
        }

        int j = i + 1, n = json_text.size();
        while (j < n && json_text[j] != '"')
            ++j;
        if (j == n)
        {
            cerr << "error:Expect right quotation." << endl;
            return;
        }
        if (j == i + 1)
        {
            cerr << "error:Empty key." << endl;
            return;
        }
        members.emplace(make_pair(key, json_value(string(json_text, i, j - i))));
    }
    void json::parsevector(const string &key, const string &json_text, int i)
    {
    }
    void json::parseobject(const string &key, const string &json_text, int i)
    {
        if (json_text[i] != '{')
        {
            cerr << "error:Expect left brace." << endl;
            return;
        }
        json subjson;
        int n = json_text.size();
        for (; i < n && json_text[i] != '}';)
        {
            skipwhitespace(json_text, i);
            if (json_text[i] != '"')
            {
                cerr << "error:Expect left quotation." << endl;
                return;
            }

            int j = i + 1;
            while (j < n && json_text[j] != '"')
                ++j;
            if (j == n)
            {
                cerr << "error:Expect right quotation." << endl;
                return;
            }
            if (j == i + 1)
            {
                cerr << "error:Empty key." << endl;
                return;
            }
            string key(json_text, i + 1, j - i - 1);
            i = j + 1;

            skipwhitespace(json_text, i);
            if (i >= n || json_text[i] != ':')
            {
                cerr << "error:Expect colon." << endl;
                return;
            }

            skipwhitespace(json_text, i);
            if (key.empty()) //根据key是否为空判断，对象是否是其他对象的子对象。
                parsevalue(key, json_text, i);
            else
                subjson.parsevalue(key, json_text, i);
            skipwhitespace(json_text, i);

            if (i < n - 1 && json_text[i] != ',')
            {
                cerr << "error:Expect comma." << endl;
            }
            ++i;
        }

        skipwhitespace(json_text, i);
        if (i == n || json_text[i] != '}')
        {
            cerr << "error:Expect right brace." << endl;
            return;
        }
        if (!key.empty())
            members.emplace(make_pair(key, subjson));
    }
    void json::parsevalue(const string &key, const string &json_text, int i)
    {
        switch (json_text[i])
        {
        case 't':
            parsebool(key, json_text, i, "true");
        case 'f':
            parsebool(key, json_text, i, "false");
        case 'n':
            parsenull(key, json_text, i, "null");
        default:
            parsenum(key, json_text, i);
        case '"':
            parsestring(key, json_text, i);
        case '{':
            parseobject(key, json_text, i);
        case '[':
            parsevector(key, json_text, i);
        }
    }

    json::json(const string &json_text, int i = 0)
    {
        int n = json_text.size();
        skipwhitespace(json_text, i);
        if (json_text[i] != '{')
        {
            cerr << "error:Expect left brace." << endl;
            return;
        }
        parseobject(string(), json_text, i);
    }
} // namespace sg
