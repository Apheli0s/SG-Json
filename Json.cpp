#include "json.h"

namespace sg
{
    inline bool isdigit(char c) { return c >= '0' && c <= '9' }

    inline void skipblank(string &s, int i)
    {
        while (s[i] == ' ')
            ++i;
    }

    void json::parsebool(const string &key, const string &json_text, int i, const char *context)
    {
        int offset = 0;
        for (; context[offset]; ++offset)
        {
            if (json_text[i + offset] != context)
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
            if (json_text[i + offset] != context)
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
        members[key] = stod(string(begin(json_text) + start, begin(json_text) + i - 1));
    }
    void json::parsestring(const string &key, const string &json_text, int i);
    {
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
        members[key] = move(string(json_text, i + 1, j - i - 1));
    }
    void parsevector(const string &key, const string &json_text, int i)
    {
        if (json_text[i] != '[')
        {
            cerr << "error:Expect left square." << endl;
            return;
        }
    }
    void parseobject(const string &json_text, int i)
    {
        if (json_text[i] != '{')
        {
            cerr << "error:Expect left brace." << endl;
            return;
        }
        while (i < n && json_text[i] != '}')
        {
            skipblank(key, json_text, i);
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

            skipblank(json_text, i);
            if (i >= n || json_text[i] != ':')
            {
                cerr << "error:Expect colon." << endl;
                return;
            }

            skipblank(json_text, i);
            parsevalue(key, json_text, i);
            skipblank(json_text, i);

            if (i < n - 1 && json_text[i] != ',')
            {
                "error:Expect comma."
            }
            ++i;
        }

        skipblank(json_text, i);
        if (i == n || json_text[i] != '}')
        {
            cerr << "error:Expect right brace." << endl;
            return;
        }
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
            return parsenum(key, json_text, i);
        case '"':
            parsestring(key, json_text, i);
        case '{':
            parseobject(key, json_text, i);
        case '[':
            parsevector(key, json_text, i);
        }
    }

    json::json(string json_text)
    {
        int i = 0, n = json_text.size();
        skipblank(key, json_text, i);
        if (json_text[i] != '{')
        {
            cerr << "error:Expect left brace." << endl;
            return;
        }
        parseobject(json_text, i);
    }
} // namespace sg
