#include "utils.h"

namespace clang {
namespace tidy {
namespace bz12 {

string GetExecutablePath(const char *Argv0) 
{
    // This just needs to be some symbol in the binary; C++ doesn't
    // allow taking the address of ::main however.
    void *P = (void *)(intptr_t)GetExecutablePath;
    return llvm::sys::fs::getMainExecutable(Argv0, P);
}

string GetCheckerConfigFilePath(const char *argv0)
{
	string res;
	string main_exec_path = GetExecutablePath(argv0);
	if (main_exec_path.empty())
	{
		return res;
	}
	string exec_file_dir = llvm::sys::path::parent_path(main_exec_path).str();

	if (exec_file_dir.empty())
	{
		return res;
	}
	if (*(exec_file_dir.end() - 1) != '/')
	{
		exec_file_dir.append("/");
	}
	res = exec_file_dir + "../config/polarity_settings.json";
	return res;
}

bool pattern_matchs(std::string reg_string, std::string pattern) {
    llvm::Regex rg = llvm::Regex(pattern);
    if(rg.match(reg_string)) {
        return true;
    }
    return false;
}

bool fuzzy_matchs(std::string fuzz_string, int start_pos, int diff_count, std::string pattern, int & start_pos2) {
    string name_sub = fuzz_string.substr(start_pos, diff_count);
    if(name_sub.length() > pattern.length()) {
        return false;
    }

    if(pattern.size() >= fuzz_string.size()) {
        return false;
    }

    int len_pattern = pattern.length();
    int len_name_sub = name_sub.length();
    int len_pattern_char = 0;
    int i, j;

    for(i=0; i<len_pattern; i++) {
        if(pattern[i] != '*') {
            len_pattern_char ++;
        }
    }

    // 从前往后查找，查找name_sub是否满足pattern
    vector<int> vec_pos;
    for(i=0; i <= len_pattern - len_name_sub; i++) {
        for(j=0; j<len_name_sub; j++) {
            if(pattern[i+j] == name_sub[j] || pattern[i+j] == '*') {
                ;
            } else {
                break;
            }
        }
        if(j == len_name_sub) {
            vec_pos.push_back(i);
        }
    }

    if(vec_pos.size() == 0) {
        return false;
    }
    for(i=0; i<fuzz_string.size()-pattern.size(); i++) {
        for(j=0; j<pattern.size(); j++) {
            if(fuzz_string[i+j] == pattern[j] || pattern[j] == '*') {
                ;
            } else {
                break;
            }
        }
        if(j == pattern.size()){
            for(size_t index=0; index<vec_pos.size(); index++) {
                if(start_pos-vec_pos[index] == i) {
                    return true;
                }
            }
        }
    }
    return false;
}

string read_nested_variable_check_mode()
{
  string mode = "defalut";
  fstream file(GetCheckerConfigFilePath("clang-tidy"), ios::in);
  if (!file.is_open()) {
    return "defalut";
  }
  string rawJsonData((istreambuf_iterator<char>(file)),
                     istreambuf_iterator<char>());
  file.close();

  rapidjson::Document doc;
  if (!doc.Parse(rawJsonData.c_str()).HasParseError()) {
    if (doc.HasMember("nested_variable_check_mode") &&
        doc["nested_variable_check_mode"].IsString() &&
        doc["nested_variable_check_mode"].GetString()!="") {
      mode = doc["nested_variable_check_mode"].GetString();
      return mode;
    }
  }
  return "defalut";
}

void read_negative_string_vector(vector<string> & negative_string_vector)
{
    fstream file(GetCheckerConfigFilePath("clang-tidy"), ios::in);
    if (!file.is_open()) {
        return;
    }
    string rawJsonData((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    rapidjson::Document doc;
    if (!doc.Parse(rawJsonData.c_str()).HasParseError()) {
        if (doc.HasMember("negative_string_vector") && doc["negative_string_vector"].IsArray()) {
            const rapidjson::Value & array = doc["negative_string_vector"];
            for (int i=0; i < array.Size(); ++i) {
                negative_string_vector.push_back(array[i].GetString());
            }
        }
    }
}

void read_double_assign_negative_vector(vector<string> & double_assign_arr_negative_vec)
{
    fstream file(GetCheckerConfigFilePath("clang-tidy"), ios::in);
    if (!file.is_open()) {
        return;
    }
    string rawJsonData((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    rapidjson::Document doc;
    if (!doc.Parse(rawJsonData.c_str()).HasParseError()) {
        if (doc.HasMember("double_assign_arr_negative_vec") && doc["double_assign_arr_negative_vec"].IsArray()) {
            const rapidjson::Value & array = doc["double_assign_arr_negative_vec"];
            for (int i=0; i < array.Size(); ++i) {
                double_assign_arr_negative_vec.push_back(array[i].GetString());
            }
        }
    }
}

void read_custom_string_set (set<string> & custom_string_set)
{
    fstream file(GetCheckerConfigFilePath("clang-tidy"), ios::in);
    if (!file.is_open())
    {
        return;
    }

    string rawJsonData((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    rapidjson::Document doc;
    if (!doc.Parse(rawJsonData.c_str()).HasParseError()) {
        if (doc.HasMember("custom_string_map") && doc["custom_string_map"].IsArray()) {
            const rapidjson::Value & array = doc["custom_string_map"];
            for (int i=0; i < array.Size(); ++i) {
                if(array[i].IsObject()) {
                    for(rapidjson::Value::ConstMemberIterator itr=array[i].MemberBegin(); itr!=array[i].MemberEnd(); ++itr) {
                        if(itr->value.IsString() && itr->value.GetString() != "") {
                            custom_string_set.insert(itr->value.GetString());
                        }
                    }
                }
            }
        }
    }
    // if (!doc.Parse(rawJsonData.c_str()).HasParseError()) {
    //     if (doc.HasMember("custom_string_set") && doc["custom_string_set"].IsArray()) {
    //         const rapidjson::Value & array = doc["custom_string_set"];
    //         for (int i=0; i < array.Size(); ++i) {
    //             custom_string_set.insert(array[i].GetString());
    //         }
    //     }
    // }
}

void read_subname_map(map<string, string> &sub_name_group_map, 
                        map<string, string> &sub_name_level_map, 
                        map<string, map<string, string>> & group_map) {
    fstream file(GetCheckerConfigFilePath("clang-tidy"), ios::in);
    if (!file.is_open())
    {
        return;
    }

    string rawJsonData((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    rapidjson::Document doc;
    if (!doc.Parse(rawJsonData.c_str()).HasParseError()) {
        if (doc.HasMember("default_string_map") && doc["default_string_map"].IsArray()) {
            const rapidjson::Value & array = doc["default_string_map"];
            for (int i=0; i < array.Size(); ++i) {
                if(array[i].IsObject()) {
                    string group_name = "group_" + to_string(i);
                    map<string, string> name_map;
                    for(rapidjson::Value::ConstMemberIterator itr=array[i].MemberBegin(); itr!=array[i].MemberEnd(); ++itr) {
                        if(itr->value.IsString() && itr->value.GetString() != "") {
                            if(itr->name.IsString() && itr->name.GetString() != "") {
                                sub_name_group_map[itr->value.GetString()] = group_name;
                                sub_name_level_map[itr->value.GetString()] = itr->name.GetString();
                                name_map[itr->name.GetString()] = itr->value.GetString();
                            }
                        }
                    }
                    if(!name_map.empty()) {
                        group_map[group_name] = name_map;
                    }
                }
            }
        }
        if (doc.HasMember("custom_string_map") && doc["custom_string_map"].IsArray()) {
            const rapidjson::Value & array = doc["custom_string_map"];
            for (int i=0; i < array.Size(); ++i) {
                if(array[i].IsObject()) {
                    string group_name = "cgroup_" + to_string(i);
                    map<string, string> name_map;
                    for(rapidjson::Value::ConstMemberIterator itr=array[i].MemberBegin(); itr!=array[i].MemberEnd(); ++itr) {
                        if(itr->value.IsString() && itr->value.GetString() != "") {
                            if(itr->name.IsString() && itr->name.GetString() != "") {
                                sub_name_group_map[itr->value.GetString()] = group_name;
                                sub_name_level_map[itr->value.GetString()] = itr->name.GetString();
                                name_map[itr->name.GetString()] = itr->value.GetString();
                            }
                        }
                    }
                    if(!name_map.empty()) {
                        group_map[group_name] = name_map;
                    }
                }
            }
        }
    }

}

void read_custom_keyword_levelset_map (map<string, set<string>> &keyword_levelset_map)
{
    fstream file(GetCheckerConfigFilePath("clang-tidy"), ios::in);
    if (!file.is_open()) {
        return;
    }
    string rawJsonData((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    rapidjson::Document doc;
    if (!doc.Parse(rawJsonData.c_str()).HasParseError()) {
        if (doc.HasMember("custom_string_map") && doc["custom_string_map"].IsArray()) {
            const rapidjson::Value & array = doc["custom_string_map"];
            for (int i=0; i < array.Size(); ++i) {
                if(array[i].IsObject()) {
                    for(rapidjson::Value::ConstMemberIterator itr=array[i].MemberBegin(); itr!=array[i].MemberEnd(); ++itr) {
                        if(itr->name.IsString() && itr->name.GetString() != "" && itr->value.IsString() && itr->value.GetString() != "") {
                            for(rapidjson::Value::ConstMemberIterator subitr=array[i].MemberBegin(); subitr!=array[i].MemberEnd(); ++subitr)
                            {
                                keyword_levelset_map[itr->value.GetString()].insert(subitr->name.GetString());
                            }
                        }
                    }
                }
            }
        }
    }
}

void read_default_keyword_levelset_map (map<string, set<string>> &keyword_levelset_map)
{
    fstream file(GetCheckerConfigFilePath("clang-tidy"), ios::in);
    if (!file.is_open()) {
        return;
    }
    string rawJsonData((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    rapidjson::Document doc;
    if (!doc.Parse(rawJsonData.c_str()).HasParseError()) {
        if (doc.HasMember("default_string_map") && doc["default_string_map"].IsArray()) {
            const rapidjson::Value & array = doc["default_string_map"];
            for (int i=0; i < array.Size(); ++i) {
                if(array[i].IsObject()) {
                    for(rapidjson::Value::ConstMemberIterator itr=array[i].MemberBegin(); itr!=array[i].MemberEnd(); ++itr) {
                        if(itr->name.IsString() && itr->name.GetString() != "" && itr->value.IsString() && itr->value.GetString() != "") {
                            for(rapidjson::Value::ConstMemberIterator subitr=array[i].MemberBegin(); subitr!=array[i].MemberEnd(); ++subitr)
                            {
                                keyword_levelset_map[itr->value.GetString()].insert(subitr->name.GetString());
                            }
                        }
                    }
                }
            }
        }
    }
}

void read_default_string_set (set<string> & default_string_set)
{
    fstream file(GetCheckerConfigFilePath("clang-tidy"), ios::in);
    if (!file.is_open())
    {
        return;
    }

    string rawJsonData((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    rapidjson::Document doc;
    if (!doc.Parse(rawJsonData.c_str()).HasParseError()) {
        if (doc.HasMember("default_string_map") && doc["default_string_map"].IsArray()) {
            const rapidjson::Value & array = doc["default_string_map"];
            for (int i=0; i < array.Size(); ++i) {
                if(array[i].IsObject()) {
                    for(rapidjson::Value::ConstMemberIterator itr=array[i].MemberBegin(); itr!=array[i].MemberEnd(); ++itr) {
                        if(itr->value.IsString() && itr->value.GetString() != "") {
                            default_string_set.insert(itr->value.GetString());
                        }
                    }
                }
            }
        }
    }

    // if (!doc.Parse(rawJsonData.c_str()).HasParseError()) {
    //     if (doc.HasMember("default_string_set") && doc["default_string_set"].IsArray()) {
    //         const rapidjson::Value & array = doc["default_string_set"];
    //         for (int i=0; i < array.Size(); ++i) {
    //             default_string_set.insert(array[i].GetString());
    //         }
    //     }
    // }
}


void read_custom_string_level_map (map<string, set<string>> & custom_string_level_map)
{
    fstream file(GetCheckerConfigFilePath("clang-tidy"), ios::in);
    if (!file.is_open()) {
        return;
    }
    string rawJsonData((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    rapidjson::Document doc;
    if (!doc.Parse(rawJsonData.c_str()).HasParseError()) {
        if (doc.HasMember("custom_string_map") && doc["custom_string_map"].IsArray()) {
            const rapidjson::Value & array = doc["custom_string_map"];
            for (int i=0; i < array.Size(); ++i) {
                if(array[i].IsObject()) {
                    for(rapidjson::Value::ConstMemberIterator itr=array[i].MemberBegin(); itr!=array[i].MemberEnd(); ++itr) {
                        if(itr->name.IsString() && itr->name.GetString() != "" && itr->value.IsString() && itr->value.GetString() != "") {
                            custom_string_level_map[itr->name.GetString()].insert(itr->value.GetString());
                        }
                    }
                }
            }
        }
    }
    // if (!doc.Parse(rawJsonData.c_str()).HasParseError()) {
    //     if (doc.HasMember("custom_string_level_map") && doc["custom_string_level_map"].IsObject()) {
    //         const rapidjson::Value & object = doc["custom_string_level_map"];
    //         for(rapidjson::Value::ConstMemberIterator itr=object.MemberBegin(); itr!=object.MemberEnd(); ++itr) {
    //             const rapidjson::Value &val_array = itr->value;
    //             set<string> set1;
    //             if (val_array.IsArray()) {
    //                 for (int j = 0; j < val_array.Size(); ++j) {
    //                     set1.insert(val_array[j].GetString());
    //                 }
    //             }
    //             if(itr->name.GetString() != "") {
    //                 custom_string_level_map[itr->name.GetString()] = set1;
    //             }

    //         }
    //     }
    // }
}
 void read_default_string_level_map (map<string, set<string>> & default_string_level_map)
{
    fstream file(GetCheckerConfigFilePath("clang-tidy"), ios::in);
    if (!file.is_open()) {
        return;
    }
    string rawJsonData((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    rapidjson::Document doc;
    if (!doc.Parse(rawJsonData.c_str()).HasParseError()) {
        if (doc.HasMember("default_string_map") && doc["default_string_map"].IsArray()) {
            const rapidjson::Value & array = doc["default_string_map"];
            for (int i=0; i < array.Size(); ++i) {
                if(array[i].IsObject()) {
                    for(rapidjson::Value::ConstMemberIterator itr=array[i].MemberBegin(); itr!=array[i].MemberEnd(); ++itr) {
                        if(itr->name.IsString() && itr->name.GetString() != "" && itr->value.IsString() && itr->value.GetString() != "") {
                            default_string_level_map[itr->name.GetString()].insert(itr->value.GetString());
                        }
                    }
                }
            }
        }
    }
    // if (!doc.Parse(rawJsonData.c_str()).HasParseError()) {
    //     if (doc.HasMember("default_string_level_map") && doc["default_string_level_map"].IsObject()) {
    //         const rapidjson::Value & object = doc["default_string_level_map"];
    //         for(rapidjson::Value::ConstMemberIterator itr=object.MemberBegin(); itr!=object.MemberEnd(); ++itr) {
    //             const rapidjson::Value &val_array = itr->value;
    //             set<string> set1;
    //             if (val_array.IsArray()) {
    //                 for (int j = 0; j < val_array.Size(); ++j) {
    //                     set1.insert(val_array[j].GetString());
    //                 }
    //             }
    //             if(itr->name.GetString() != "") {
    //                 default_string_level_map[itr->name.GetString()] = set1;
    //             }

    //         }
    //     }
    // }
}

bool read_custom_case_sensitive ()
{
    bool bo = false;
    fstream file(GetCheckerConfigFilePath("clang-tidy"), ios::in);
    if (!file.is_open())
    {
        return false;
    }

    string rawJsonData((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    rapidjson::Document doc;
    if(!doc.Parse(rawJsonData.c_str()).HasParseError()) {
        if (doc.HasMember("custom_case_sensitive") && doc["custom_case_sensitive"].IsBool()) 
        {
          bo=doc["custom_case_sensitive"].GetBool();
          return bo;
        }
    }
    return bo;
}

void read_mutex_level_map (map<string, string> &mutex_level_map)
{
    fstream file(GetCheckerConfigFilePath("clang-tidy"), ios::in);
    if (!file.is_open())
    {
        return;
    }

    string rawJsonData((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    rapidjson::Document doc;
    if (!doc.Parse(rawJsonData.c_str()).HasParseError()) {
        if (doc.HasMember("mutex_string_map") && doc["mutex_string_map"].IsObject()) {
            for(rapidjson::Value::ConstMemberIterator itr=doc["mutex_string_map"].MemberBegin(); itr!=doc["mutex_string_map"].MemberEnd(); ++itr) {
                if(itr->name.IsString() && itr->name.GetString() != "" && itr->value.IsArray()) {
                    string level_name = itr->name.GetString();
                    const rapidjson::Value & array = itr->value;
                    for (int i=0; i < array.Size(); ++i) {
                        if(array[i] != "") {
                            mutex_level_map[array[i].GetString()] = level_name;
                        }
                    }
                }
            }
        }
    }
}

void processJsonMember(const rapidjson::Value &memberValue, map<string, vector<double>> &expected_value_map, string varName)
{
    static bool clearFlag = false;
    if(memberValue.IsNumber())
    {
        expected_value_map[varName].push_back(memberValue.GetDouble());
    }
    else if(memberValue.IsArray())
    {
        for(int i=0; i < memberValue.Size(); ++i) 
        {
            processJsonMember(memberValue[i], expected_value_map, varName);
        }
    }
    else if(memberValue.IsObject())
    {
        for(auto iter=memberValue.MemberBegin(); iter!=memberValue.MemberEnd(); iter++)
        {   
            clearFlag = true;
            varName += "." + (string)(*iter).name.GetString();
            processJsonMember((*iter).value, expected_value_map, varName);
            if(clearFlag)
            {
                if(varName.find('.') != string::npos)
                {
                    //cout << "target:" << varName << endl;
                    int i = varName.length()-1;
                    while(varName[i] != '.')
                    {
                        varName.pop_back();
                        i--;
                    }
                    varName.pop_back();
                    //cout << "proed:" << varName << endl;
                }
                clearFlag = false;
            }
        }
    }
    else
    {
        //doNothing
    }
}

void read_expected_value_map(map<string, vector<double>> &expected_value_map)
{
    fstream file(GetCheckerConfigFilePath("clang-tidy"), ios::in);
    if (!file.is_open()) 
    {
        return;
    }
    string rawJsonData((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    rapidjson::Document doc;
    if (!doc.Parse(rawJsonData.c_str()).HasParseError()) 
    {
        for(auto iter=doc.MemberBegin(); iter!=doc.MemberEnd(); iter++)
        {
            processJsonMember((*iter).value, expected_value_map, (*iter).name.GetString());
        }
    }
    // for(auto iter=expected_value_map.begin(); iter!=expected_value_map.end(); iter++)
    // {
    //     cout << "***************" << endl;
    //     cout << "name:" << iter->first << endl;
    //     for(auto subIter=iter->second.begin(); subIter!=iter->second.end(); subIter++)
    //     {
    //         cout << *subIter << endl;
    //     }
    // }
}

void read_struct_alignment(int &alignment) {
    alignment = 0;
    fstream file(GetCheckerConfigFilePath("clang-tidy"), ios::in);
    if (!file.is_open())
    {
        return;
    }

    string rawJsonData((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    rapidjson::Document doc;
    if (!doc.Parse(rawJsonData.c_str()).HasParseError()) {
        if (doc.HasMember("struct_default_alignment") && doc["struct_default_alignment"].IsInt()) {
            alignment = doc["struct_default_alignment"].GetInt();
        }
    }
}

string get_default_string_level(string key_word, map<string,set<string>> default_string_level_map) {
    string lower_key_word = to_lower_string(key_word);
    map<string,set<string>>::iterator it_begin = default_string_level_map.begin(), it_end = default_string_level_map.end();
    for( ; it_begin != it_end; ++it_begin) {
        set<string>::iterator sit_begin = it_begin->second.begin(), sit_end = it_begin->second.end();
        for(; sit_begin != sit_end; sit_begin++) {
            string lower_name = to_lower_string(*sit_begin);
            if(lower_name == lower_key_word) {
                return it_begin->first;
            }
        }
    } 
    return "";
}

string get_custom_string_level(string key_word, map<string,set<string>> custom_string_level_map) {
    bool custom_case_sensitive = read_custom_case_sensitive();
    if(custom_case_sensitive)
    {
        map<string,set<string>>::iterator it_begin = custom_string_level_map.begin(), it_end = custom_string_level_map.end();
        for( ; it_begin != it_end; ++it_begin) {
            set<string>::iterator sit_begin = it_begin->second.begin(), sit_end = it_begin->second.end();
            for(; sit_begin != sit_end; sit_begin++) {
                if(*sit_begin == key_word) {
                    return it_begin->first;
                }
            }
        }
        return "";
    }
    else
    {
        string lower_key_word = to_lower_string(key_word);
        map<string,set<string>>::iterator it_begin = custom_string_level_map.begin(), it_end = custom_string_level_map.end();
        for( ; it_begin != it_end; ++it_begin) {
            set<string>::iterator sit_begin = it_begin->second.begin(), sit_end = it_begin->second.end();
            for(; sit_begin != sit_end; sit_begin++) {
                string lower_name = to_lower_string(*sit_begin);
                if(lower_name == lower_key_word) {
                    return it_begin->first;
                }
            }
        }
        return "";
    }
}

string to_lower_string(string str) {
    string lower_string = "";
    for(size_t i=0; i<str.size(); i++) {
        lower_string += tolower(str[i]);
    }
    return lower_string;
}

bool case_insensitive_find_in_vector(string key_word, vector<string> vec_str, size_t &index) {
    string key_word_lower = to_lower_string(key_word);
    size_t for_index = 0;
    for(; for_index<vec_str.size(); for_index++) {
        if(key_word_lower == to_lower_string(vec_str[for_index])) {
            break;
        }
    }
    if(for_index==vec_str.size()) {
        index = 0;
        return false;
    } else {
        index = for_index;
        return true;
    }
}

bool case_insensitive_find_in_negative_vector(string key_word, vector<string> vec_str, size_t &index) {
    string key_word_lower = to_lower_string(key_word);
    size_t for_index = 0;
    for(; for_index<vec_str.size(); for_index++) {
        // key_word_lower.find()
        string index_tring = to_lower_string(vec_str[for_index]);
        if(key_word_lower.find(index_tring) != key_word_lower.npos) {
            break;
        }
    }
    if(for_index==vec_str.size()) {
        index = 0;
        return false;
    } else {
        index = for_index;
        return true;
    }
}

bool case_insensitive_find_in_set(string key_word, set<string> set_str, string set_key_word) {
    string key_word_lower = to_lower_string(key_word);
    set<string>::iterator it_begin = set_str.begin(), it_end = set_str.end();
    for(; it_begin!=it_end; it_begin++) {
        if(key_word_lower == to_lower_string(*it_begin)) {
            break;
        }
    }
    if(it_begin == it_end) {
        set_key_word = "";
        return false;
    } else {
        set_key_word = *it_begin;
        return true;
    }
}

// 升序
void transform_default_string_set_to_list(list<string> & default_string_list_sorted_by_lenth, set<string> default_string_set) {
    default_string_list_sorted_by_lenth.clear();
    set<string>::iterator it;
    list<string>::iterator lit;
    for(it=default_string_set.begin(); it!=default_string_set.end(); it++) {
        for(lit=default_string_list_sorted_by_lenth.begin(); lit!=default_string_list_sorted_by_lenth.end(); ++lit){
            if(lit->length() > it->length()) {
                break;
            }
        } 
        default_string_list_sorted_by_lenth.insert(lit, *it);
    }
}

void transform_custom_string_set_to_list(list<string> & custom_string_list_sorted_by_lenth, set<string> custom_string_set) {
    custom_string_list_sorted_by_lenth.clear();
    set<string>::iterator it;
    list<string>::iterator lit;
    for(it=custom_string_set.begin(); it!=custom_string_set.end(); it++) {
        for(lit=custom_string_list_sorted_by_lenth.begin(); lit!=custom_string_list_sorted_by_lenth.end(); ++lit){
            if(lit->length() > it->length()) {
                break;
            }
        } 
        custom_string_list_sorted_by_lenth.insert(lit, *it);
    }
}

bool analysisNameSimilarity(
    list<string> default_string_list_sorted_by_lenth, list<string> custom_string_list_sorted_by_lenth, 
    string name1, string name2, string & sub_name1, string & sub_name2, int & start_pos1, int & start_pos2) {
    if(name1.length() < 2 || name2.length() < 2) //只考虑长度大于2
    {
        return false;
    }
    int start_count = 0, end_count = 0;

    size_t lenth_name1 = name1.length();
    size_t lenth_name2 = name2.length();
    size_t min_lenth = lenth_name1 < lenth_name2 ? lenth_name1:lenth_name2;

    for(size_t i=0; i < min_lenth; i++) {
        if(name1[i] == name2[i]){
            start_count++;
        } else {
            break;
        }
    }

    for(size_t i=0; i < min_lenth; i++){
        if(name1[lenth_name1 - i - 1] == name2[lenth_name2 - i - 1]){
            end_count++;
        } else {
            break;
        }
    }

    if(start_count == 0 && end_count == 0 ) {
        return false;
    }

    int name1_diff_char_count = name1.length() - start_count - end_count;
    int name2_diff_char_count = name2.length() - start_count - end_count;

    if(name1_diff_char_count < 0 || name2_diff_char_count < 0 ) {
        return false;
    }

    string name1_sub = name1.substr(start_count, name1_diff_char_count);
    string name2_sub = name2.substr(start_count, name2_diff_char_count);
    bool sign_name1 = false;
    bool sign_name2 = false;

    if(name1_sub == "" || name2_sub == "") 
        return false;

    if(start_count > 2 && name1[start_count-1] == '+' && name1[start_count-2] == '+') {
        string arr_index = name1.substr(start_count);
        int index = 0;
        for(; index!=arr_index.size(); index++) {
            if(arr_index[index] < '0' || arr_index[index] > '9') {
                break;
            }
        }
        if(index==arr_index.size() && index != 0) {
            int int_arr_index = atoi(arr_index.c_str());
            if(int_arr_index >= 0) {
                name1_sub = "arrayindex" + to_string(int_arr_index);
            }
        }
    }

    if(start_count > 2 && name2[start_count-1] == '+' && name2[start_count-2] == '+') {
        string arr_index = name2.substr(start_count);
        int index = 0;
        for(; index!=arr_index.size(); index++) {
            if(arr_index[index] < '0' || arr_index[index] > '9') {
                break;
            }
        }
        if(index==arr_index.size() && index != 0) {
            int int_arr_index = atoi(arr_index.c_str());
            if(int_arr_index >= 0) {
                name2_sub = "arrayindex" + to_string(int_arr_index);
            }
        }
    }

    bool custom_case_sensitive = read_custom_case_sensitive();
    for(list<string>::reverse_iterator lit=custom_string_list_sorted_by_lenth.rbegin(); lit!=custom_string_list_sorted_by_lenth.rend(); lit++) {
        if(!pattern_matchs(*lit)) {
            if(custom_case_sensitive) {
                if((*lit == name1_sub) && (sign_name1 == false)) {
                    sub_name1 = *lit;
                    sign_name1 = true;
                    start_pos1 = start_count;
                } else if(lit->find(name1_sub) != lit->npos && sign_name1 == false) {
                    if(name1.find(*lit) != name1.npos && sign_name1 == false) {
                        sub_name1 = *lit;
                        sign_name1 = true;
                        start_pos1 = name1.find(*lit);
                    }
                } else { }

                if((*lit == name2_sub) && (sign_name2 == false)) {
                    sub_name2 = *lit;
                    sign_name2 = true;
                    start_pos2 = start_count;
                } else if((lit->find(name2_sub) != lit->npos) && (sign_name2 == false)) {
                    if(name2.find(*lit) != name2.npos && sign_name2 == false) {
                        sub_name2 = *lit;
                        sign_name2 = true;
                        start_pos1 = name2.find(*lit);
                    }
                } else { }

                //如果找到的两个变量找到的关键字是一样的，那么就不算，继续找
                if(sign_name1 && sign_name2 && (sub_name1==sub_name2))
                {
                    sign_name1 = false;
                    sign_name2 = false;
                    continue;
                }
            } else {
                string lower_string = to_lower_string(*lit);
                string lower_name1_sub = to_lower_string(name1_sub);
                string lower_name1 = to_lower_string(name1);
                string lower_name2_sub = to_lower_string(name2_sub);
                string lower_name2 = to_lower_string(name2);

                if((lower_string == lower_name1_sub) && (sign_name1 == false)) {
                    sub_name1 = *lit;
                    sign_name1 = true;
                    start_pos1 = start_count;
                } else if((lower_string.find(lower_name1_sub) != lower_string.npos) && (sign_name1 == false)) {
                    if(lower_name1.find(lower_string) != lower_name1.npos) {
                        sub_name1 = *lit;
                        sign_name1 = true;
                        start_pos1 = lower_name1.find(lower_string);
                    }
                } else { }

                if(lower_string == lower_name2_sub) {
                    sub_name2 = *lit;
                    sign_name2 = true;
                    start_pos2 = start_count;
                } else if(lower_string.find(lower_name2_sub) != lower_string.npos) {
                    if(lower_name2.find(lower_string) != lower_name2.npos) {
                        sub_name2 = *lit;
                        sign_name2 = true;
                        start_pos2 = lower_name2.find(lower_string);
                    }
                } else { }
                 //如果找到的两个变量找到的关键字是一样的，那么就不算，继续找
                if(sign_name1 && sign_name2 && (sub_name1==sub_name2))
                {
                    sign_name1 = false;
                    sign_name2 = false;
                    continue;
                }
            }
        } else {
            if(fuzzy_matchs(name1, start_count, name1_diff_char_count, *lit, start_pos1) && sign_name1==false) {
                sub_name1 = *lit;
                sign_name1 = true;
            }
            if(fuzzy_matchs(name2, start_count, name2_diff_char_count, *lit, start_pos2) && sign_name2==false) {
                sub_name2 = *lit;
                sign_name2 = true;
            }
            //如果找到的两个变量找到的关键字是一样的，那么就不算，继续找
            if(sign_name1 && sign_name2 && (sub_name1==sub_name2))
            {
                sign_name1 = false;
                sign_name2 = false;
                continue;
            }
        }
    }


    for(list<string>::reverse_iterator lit=default_string_list_sorted_by_lenth.rbegin(); lit!=default_string_list_sorted_by_lenth.rend(); lit++) {
        string lower_string = to_lower_string(*lit);
        string lower_name1_sub = to_lower_string(name1_sub);
        string lower_name1 = to_lower_string(name1);
        string lower_name2_sub = to_lower_string(name2_sub);
        string lower_name2 = to_lower_string(name2);

        if(lower_string == lower_name1_sub && sign_name1 == false) {
            sub_name1 = *lit;
            sign_name1 = true;
            start_pos1 = start_count;
        } else if(lower_string.find(lower_name1_sub) != lower_string.npos) {
            if(lower_name1.find(lower_string) != lower_name1.npos) {
                if(sign_name1 == false) {
                    sub_name1 = *lit;
                    sign_name1 = true;
                    start_pos1 = lower_name1.find(lower_string);
                }
            }
        } else { }

        //如果找到的两个变量找到的关键字是一样的，那么就不算，继续找
        if(sign_name1 && sign_name2 && (sub_name1==sub_name2))
        {
            sign_name1 = false;
            sign_name2 = false;
            continue;
        }

        if(lower_string == lower_name2_sub&& sign_name2 == false) {
            sub_name2 = *lit;
            sign_name2 = true;
            start_pos2 = start_count;
        } else if(lower_string.find(lower_name2_sub) != lower_string.npos) {
            if(lower_name2.find(lower_string) != lower_name2.npos) {
                if(sign_name2 == false) {
                    sub_name2 = *lit;
                    sign_name2 = true;
                    start_pos2 = lower_name2.find(lower_string);
                }
            }
        } else { }

        //如果找到的两个变量找到的关键字是一样的，那么就不算，继续找
        if(sign_name1 && sign_name2 && (sub_name1==sub_name2))
        {
            sign_name1 = false;
            sign_name2 = false;
            continue;
        }
    }

    if(sign_name1 && sign_name2 && (sub_name1!=sub_name2)) {
        return true;
    }
    return false;
}


bool analysisArraySubNameSimilarity(
    list<string> default_string_list_sorted_by_lenth, list<string> custom_string_list_sorted_by_lenth, 
    string name1, string name2, string & sub_name1, string & sub_name2, int & start_pos1, int & start_pos2) {
    if(name1.length() < 2 || name2.length() < 2) //只考虑长度大于2
    {
        return false;
    }
    int start_count = 0, end_count = 0;

    size_t lenth_name1 = name1.length();
    size_t lenth_name2 = name2.length();
    size_t min_lenth = lenth_name1 < lenth_name2 ? lenth_name1:lenth_name2;

    for(size_t i=0; i < min_lenth; i++) {
        if(name1[i] == name2[i]){
            start_count++;
        } else {
            break;
        }
    }

    for(size_t i=0; i < min_lenth; i++){
        if(name1[lenth_name1 - i - 1] == name2[lenth_name2 - i - 1]){
            end_count++;
        } else {
            break;
        }
    }

    if(start_count == 0 && end_count == 0 ) {
        return false;
    }

    int name1_diff_char_count = name1.length() - start_count - end_count;
    int name2_diff_char_count = name2.length() - start_count - end_count;

    if(name1_diff_char_count < 0 || name2_diff_char_count < 0 ) {
        return false;
    }

    string name1_sub = name1.substr(start_count, name1_diff_char_count);
    string name2_sub = name2.substr(start_count, name2_diff_char_count);
    bool sign_name1 = false;
    bool sign_name2 = false;

    if(name1_sub == "" || name2_sub == "") 
        return false;

    if(start_count > 2 && name1[start_count-1] == '+' && name1[start_count-2] == '+') {
        string arr_index = name1.substr(start_count);
        int index = 0;
        for(; index!=arr_index.size(); index++) {
            if(arr_index[index] < '0' || arr_index[index] > '9') {
                break;
            }
        }
        if(index==arr_index.size() && index != 0) {
            int int_arr_index = atoi(arr_index.c_str());
            if(int_arr_index >= 0) {
                name1_sub = "arrayindex" + to_string(int_arr_index);
            }
        }
    }

    if(start_count > 2 && name2[start_count-1] == '+' && name2[start_count-2] == '+') {
        string arr_index = name2.substr(start_count);
        int index = 0;
        for(; index!=arr_index.size(); index++) {
            if(arr_index[index] < '0' || arr_index[index] > '9') {
                break;
            }
        }
        if(index==arr_index.size() && index != 0) {
            int int_arr_index = atoi(arr_index.c_str());
            if(int_arr_index >= 0) {
                name2_sub = "arrayindex" + to_string(int_arr_index);
            }
        }
    }

    if(StartWith(name1_sub, "arrayindex") && StartWith(name2_sub, "arrayindex")) {
        sub_name1 = name1_sub;
        sub_name2 = name2_sub;
        start_pos1 = start_count;
        start_pos2 = start_count;
        return true;
    }

    return false;
}

bool analysisNameSimilarityV2(
    list<string> default_string_list_sorted_by_lenth, list<string> custom_string_list_sorted_by_lenth, 
    string name1, string name2, vector<string> & vec_sub_name1, vector<string> & vec_sub_name2) {
    if(name1.length() < 2 || name2.length() < 2) //只考虑长度大于2
    {
        return false;
    }
    int start_count = 0, end_count = 0;

    size_t lenth_name1 = name1.length();
    size_t lenth_name2 = name2.length();
    size_t min_lenth = lenth_name1 < lenth_name2 ? lenth_name1:lenth_name2;

    for(size_t i=0; i < min_lenth; i++) {
        if(name1[i] == name2[i]){
            start_count++;
        } else {
            break;
        }
    }

    for(size_t i=0; i < min_lenth; i++){
        if(name1[lenth_name1 - i - 1] == name2[lenth_name2 - i - 1]){
            end_count++;
        } else {
            break;
        }
    }

    if(start_count == 0 && end_count == 0 ) {
        return false;
    }

    int name1_diff_char_count = name1.length() - start_count - end_count;
    int name2_diff_char_count = name2.length() - start_count - end_count;

    if(name1_diff_char_count < 0 || name2_diff_char_count < 0 ) {
        return false;
    }

    string name1_sub = name1.substr(start_count, name1_diff_char_count);
    string name2_sub = name2.substr(start_count, name2_diff_char_count);
    bool sign_name1 = false;
    bool sign_name2 = false;

    if(name1_sub == "" || name2_sub == "") 
        return false;

    if(start_count > 2 && name1[start_count-1] == '+' && name1[start_count-2] == '+') {
        string arr_index = name1.substr(start_count);
        int index = 0;
        for(; index!=arr_index.size(); index++) {
            if(arr_index[index] < '0' || arr_index[index] > '9') {
                break;
            }
        }
        if(index==arr_index.size() && index != 0) {
            int int_arr_index = atoi(arr_index.c_str());
            if(int_arr_index >= 0) {
                name1_sub = "arrayindex" + to_string(int_arr_index);
            }
        }
    }

    if(start_count > 2 && name2[start_count-1] == '+' && name2[start_count-2] == '+') {
        string arr_index = name2.substr(start_count);
        int index = 0;
        for(; index!=arr_index.size(); index++) {
            if(arr_index[index] < '0' || arr_index[index] > '9') {
                break;
            }
        }
        if(index==arr_index.size() && index != 0) {
            int int_arr_index = atoi(arr_index.c_str());
            if(int_arr_index >= 0) {
                name2_sub = "arrayindex" + to_string(int_arr_index);
            }
        }
    }

    for(list<string>::reverse_iterator lit=default_string_list_sorted_by_lenth.rbegin(); lit!=default_string_list_sorted_by_lenth.rend(); lit++) {
        string lower_string = to_lower_string(*lit);
        string lower_name1_sub = to_lower_string(name1_sub);
        string lower_name1 = to_lower_string(name1);
        string lower_name2_sub = to_lower_string(name2_sub);
        string lower_name2 = to_lower_string(name2);

        if(lower_string == lower_name1_sub) {
            vec_sub_name1.push_back(*lit);
        } else if(lower_string.find(lower_name1_sub) != lower_string.npos) {
            if(lower_name1.find(lower_string) != lower_name1.npos) {
                vec_sub_name1.push_back(*lit);
            }
        } else { }

        if(lower_string == lower_name2_sub) {
            vec_sub_name2.push_back(*lit);
        } else if(lower_string.find(lower_name2_sub) != lower_string.npos) {
            if(lower_name2.find(lower_string) != lower_name2.npos) {
                vec_sub_name2.push_back(*lit);
            }
        } else { }
    }
    bool custom_case_sensitive = read_custom_case_sensitive();
    for(list<string>::reverse_iterator lit=custom_string_list_sorted_by_lenth.rbegin(); lit!=custom_string_list_sorted_by_lenth.rend(); lit++) {
        if(!pattern_matchs(*lit)) {
            if(custom_case_sensitive) {
                if(*lit == name1_sub) {
                    vec_sub_name1.push_back(*lit);
                } else if(lit->find(name1_sub) != lit->npos) {
                    if(name1.find(*lit) != name1.npos) {
                        vec_sub_name1.push_back(*lit);
                    }
                } else { }

                if(*lit == name2_sub) {
                    vec_sub_name2.push_back(*lit);
                } else if(lit->find(name2_sub) != lit->npos) {
                    if(name2.find(*lit) != name2.npos) {
                        vec_sub_name2.push_back(*lit);
                    }
                } else { }
            } else {
                string lower_string = to_lower_string(*lit);
                string lower_name1_sub = to_lower_string(name1_sub);
                string lower_name1 = to_lower_string(name1);
                string lower_name2_sub = to_lower_string(name2_sub);
                string lower_name2 = to_lower_string(name2);

                if(lower_string == lower_name1_sub) {
                    vec_sub_name1.push_back(*lit);
                } else if(lower_string.find(lower_name1_sub) != lower_string.npos) {
                    if(lower_name1.find(lower_string) != lower_name1.npos) {
                        vec_sub_name1.push_back(*lit);
                    }
                } else { }

                if(lower_string == lower_name2_sub) {
                    vec_sub_name2.push_back(*lit);
                } else if(lower_string.find(lower_name2_sub) != lower_string.npos) {
                    if(lower_name2.find(lower_string) != lower_name2.npos) {
                        vec_sub_name2.push_back(*lit);
                    }
                } else { }

            }
        } else {
            int start_pos_temp;
            if(fuzzy_matchs(name1, start_count, name1_diff_char_count, *lit, start_pos_temp)) {
                vec_sub_name1.push_back(*lit);
            }
            if(fuzzy_matchs(name2, start_count, name2_diff_char_count, *lit, start_pos_temp)) {
                vec_sub_name2.push_back(*lit);
            }
        }
    }

    if(vec_sub_name1.size() && vec_sub_name2.size()) {
        return true;
    }
    return false;
}

string get_group_name_prefix(string name1, string name2, string sub_name1, string sub_name2) {
    int start_count = 0, end_count = 0;

    size_t lenth_name1 = name1.length();
    size_t lenth_name2 = name2.length();
    size_t min_lenth = lenth_name1 < lenth_name2 ? lenth_name1:lenth_name2;

    for(size_t i=0; i < min_lenth; i++) {
        if(name1[i] == name2[i]){
            start_count++;
        } else {
            break;
        }
    }

    for(size_t i=0; i < min_lenth; i++){
        if(name1[lenth_name1 - i - 1] == name2[lenth_name2 - i - 1]){
            end_count++;
        } else {
            break;
        }
    }

    if(start_count == 0 && end_count == 0 ) {
        return "";
    }

    int name1_diff_char_count = name1.length() - start_count - end_count;
    int name2_diff_char_count = name2.length() - start_count - end_count;

    if(name1_diff_char_count < 0 || name2_diff_char_count < 0 ) {
        return "";
    }

    return name1.substr(0, start_count);

}


bool StartWith(std::string s1, std::string s2){
    return s1.compare(0, s2.size(), s2) == 0;
}

std::string getInterLiteralExprString(const BinaryOperator *BO){
    std::string str = "";
    if (BO) {
        const BinaryOperator *temp_bo = BO;
        if(const IntegerLiteral* IL = dyn_cast<IntegerLiteral>(temp_bo->getLHS()->IgnoreParenImpCasts())) {
            str = "++" + IL->getValue().toString(10, true) + str;
        } else if(const DeclRefExpr* DRE=dyn_cast<DeclRefExpr>(temp_bo->getLHS()->IgnoreParenImpCasts())) {
            str = "++" + DRE->getDecl()->getDeclName().getAsString() + str;
        } else {}
        if (const IntegerLiteral* IR = dyn_cast<IntegerLiteral>(temp_bo->getRHS()->IgnoreParenImpCasts())) {
            str = str + "++" + IR->getValue().toString(10, true);
        } else if(const DeclRefExpr* DRE=dyn_cast<DeclRefExpr>(temp_bo->getRHS()->IgnoreParenImpCasts())) {
            str = str + "++" + DRE->getDecl()->getDeclName().getAsString();
        } else {}
        if (const BinaryOperator * BOL = dyn_cast<BinaryOperator>(temp_bo->getLHS()->IgnoreParenImpCasts())) {
            str = getInterLiteralExprString(BOL) + str;
        }
        if (const BinaryOperator * BOR = dyn_cast<BinaryOperator>(temp_bo->getRHS()->IgnoreParenImpCasts())) {
            str = str + getInterLiteralExprString(BOR);
        }
    } 
    return str;
}

std::string getArraySubscriptExprString(const ArraySubscriptExpr * ASE){
    std::string str = "";
    if(ASE) {
        const ArraySubscriptExpr * temp_ase = ASE;
        while (true) {
            if(const IntegerLiteral* IL = dyn_cast<IntegerLiteral>(temp_ase->getRHS()->IgnoreParenImpCasts())) {
                str = "++" + IL->getValue().toString(10, true) + str;
            } else if (const DeclRefExpr *RDRE = dyn_cast<DeclRefExpr>(temp_ase->getRHS()->IgnoreParenImpCasts())) {
                str = "++" + getDeclRefExprString(RDRE) + str;
                // str = getDeclRefExprString(RDRE) + str;
            } else if (const BinaryOperator *BO = dyn_cast<BinaryOperator>(temp_ase->getRHS()->IgnoreParenImpCasts())) {
                str = getInterLiteralExprString(BO) + str;
            } else if(const UnaryOperator *UO = dyn_cast<UnaryOperator>(temp_ase->getRHS()->IgnoreParenImpCasts())){
                if(const DeclRefExpr *MatchedDeclRefExpr = dyn_cast<DeclRefExpr>(UO->getSubExpr()->IgnoreParenImpCasts())){
                    str = "++" + getDeclRefExprString(MatchedDeclRefExpr) + str;
                }
            } else {
                str = "";
                break;
            }
            if(const DeclRefExpr* DRE=dyn_cast<DeclRefExpr>(temp_ase->getLHS()->IgnoreParenImpCasts())) {
                str = DRE->getDecl()->getDeclName().getAsString() + str;
                break;
            } else if(const ArraySubscriptExpr* LASE=dyn_cast<ArraySubscriptExpr>(temp_ase->getLHS()->IgnoreParenImpCasts())) {
                temp_ase = LASE;
            } else if(const MemberExpr* LME=dyn_cast<MemberExpr>(temp_ase->getLHS()->IgnoreParenImpCasts())) {
                str = getMemberExprString(LME) + str;
                break;
            } else if(const UnaryOperator *UO = dyn_cast<UnaryOperator>(temp_ase->getLHS()->IgnoreParenImpCasts())){
                if(const DeclRefExpr *MatchedDeclRefExpr = dyn_cast<DeclRefExpr>(UO->getSubExpr()->IgnoreParenImpCasts())){
                    str =MatchedDeclRefExpr->getDecl()->getDeclName().getAsString() + str;
                }
            } else {
                break;
            }
        }
    }
    return str;
}

std::string getMemberExprString(const MemberExpr * ME){
    std::string str = "";
    if(ME) {
        const MemberExpr * temp_me = ME;
        while (true) {
            str = ".." + temp_me->getMemberDecl()->getDeclName().getAsString() + str;
            if(const DeclRefExpr* DRE=dyn_cast<DeclRefExpr>(temp_me->getBase()->IgnoreParenImpCasts())) {
                str = DRE->getDecl()->getDeclName().getAsString() + str;
                break;
            } else if(const MemberExpr* BME=dyn_cast<MemberExpr>(temp_me->getBase()->IgnoreParenImpCasts())) {
                temp_me = BME;
            } else if(const ArraySubscriptExpr* BASE=dyn_cast<ArraySubscriptExpr>(temp_me->getBase()->IgnoreParenImpCasts())) {
                str = getArraySubscriptExprString(BASE) + str; 
                break;
            } else {
                break;
            }
        }
    }
    return str;
}

std::string getDeclRefExprString(const DeclRefExpr * DRE){
    std::string str = "";
    if(DRE){
        str =  DRE->getDecl()->getDeclName().getAsString();
    }
    return str;
}


void getBinaryStringVector(const BinaryOperator *BO,
                            std::vector<std::string> &vec_str){
    std::string var_name = "", var_name2 = "";
    std::size_t i, j;
    std::string temp_name;
    const Expr * ltemp_expr = BO->getLHS()->IgnoreCasts()->IgnoreParenImpCasts();
    const Expr * rtemp_expr = BO->getRHS()->IgnoreCasts()->IgnoreParenImpCasts();
    if(const DeclRefExpr *LDRE = dyn_cast<DeclRefExpr>(ltemp_expr)){
        var_name = getDeclRefExprString(LDRE);
    } else if(const ArraySubscriptExpr *LASE = dyn_cast<ArraySubscriptExpr>(ltemp_expr)) {
        var_name = getArraySubscriptExprString(LASE);
        getArraySubscriptExprStringVector(LASE, vec_str);
    } else if(const MemberExpr *LME = dyn_cast<MemberExpr>(ltemp_expr)) {
        var_name = getMemberExprString(LME);
        getMemberExprVector(LME, vec_str);
    } else if(const BinaryOperator *LBO = dyn_cast<BinaryOperator>(ltemp_expr)) {
        getBinaryStringVector(LBO, vec_str);
    } else if(const UnaryOperator *LUO = dyn_cast<UnaryOperator>(ltemp_expr)) {
        getUnaryOperatorVector(LUO, vec_str, temp_name);
    } else {}
    if(const DeclRefExpr *RDRE = dyn_cast<DeclRefExpr>(rtemp_expr)){
        var_name2 = getDeclRefExprString(RDRE);
    } else if(const ArraySubscriptExpr *RASE = dyn_cast<ArraySubscriptExpr>(rtemp_expr)) {
        var_name2 = getArraySubscriptExprString(RASE);
        getArraySubscriptExprStringVector(RASE, vec_str);
    } else if(const MemberExpr *RME = dyn_cast<MemberExpr>(rtemp_expr)) {
        var_name2 = getMemberExprString(RME);
        getMemberExprVector(RME, vec_str);
    } else if(const BinaryOperator *RBO = dyn_cast<BinaryOperator>(rtemp_expr)) {
        getBinaryStringVector(RBO, vec_str);
    } else if(const UnaryOperator *RUO = dyn_cast<UnaryOperator>(rtemp_expr)) {
        getUnaryOperatorVector(RUO, vec_str, temp_name);
    } else {}

    if(var_name != "") {
        for(i = 0; i < vec_str.size(); i++) {
            if(vec_str[i]==var_name){
                break;
            }
        }
        if(i == vec_str.size()) {
            vec_str.push_back(var_name);
        }
    }
    if(var_name2 != "") {
        for(i = 0; i < vec_str.size(); i++) {
            if(vec_str[i]==var_name2){
                break;
            }
        }
        if(i == vec_str.size()) {
            vec_str.push_back(var_name2);
        }
    }
}

void getArraySubscriptExprStringVector(
                                    const ArraySubscriptExpr *ASE,
                                    std::vector<std::string> &vec_str){
    std::string var_name = "";
    std::string temp_name;
    std::size_t i, j;
    const Expr * temp_expr = ASE->getIdx()->IgnoreCasts()->IgnoreParenImpCasts();
    if(const DeclRefExpr *LDRE = dyn_cast<DeclRefExpr>(temp_expr)){
        var_name = getDeclRefExprString(LDRE);
    } else if(const ArraySubscriptExpr *LASE = dyn_cast<ArraySubscriptExpr>(temp_expr)) {
        var_name = getArraySubscriptExprString(LASE);
        getArraySubscriptExprStringVector(LASE, vec_str);
    } else if(const MemberExpr *LME = dyn_cast<MemberExpr>(temp_expr)) {
        var_name = getMemberExprString(LME);
        getMemberExprVector(LME, vec_str);
    } else if(const BinaryOperator *LBO = dyn_cast<BinaryOperator>(temp_expr)) {
        getBinaryStringVector(LBO, vec_str);
    } else if(const UnaryOperator *LUO = dyn_cast<UnaryOperator>(temp_expr)) {
        getUnaryOperatorVector(LUO, vec_str, temp_name);
    } else {}

    if(var_name != "") {
        for(i = 0; i < vec_str.size(); i++) {
            if(vec_str[i]==var_name){
                break;
            }
        }
        if(i == vec_str.size()) {
            vec_str.push_back(var_name);
        }
    }
}

void getCallExprStringVector(const CallExpr *CE,
                                std::vector<std::string> &vec_str,
                                std::string &arr_str) {
    unsigned NumArgs = CE->getNumArgs();
    for (unsigned i = 0; i < NumArgs; i++) {
        const Expr *RE = CE->getArg(i)->IgnoreCasts()->IgnoreParenImpCasts();
        std::string var_name = "";
        if(const DeclRefExpr *RDRE = dyn_cast<DeclRefExpr>(RE)){
            if(RDRE->getType()->isArrayType() || RDRE->getType()->isPointerType()) {
                arr_str = getDeclRefExprString(RDRE);
                var_name = getDeclRefExprString(RDRE);
            } else {
                var_name = getDeclRefExprString(RDRE);
            }
        } else if(const ArraySubscriptExpr *ASERE = dyn_cast<ArraySubscriptExpr>(RE)){
            getArraySubscriptExprStringVector(ASERE, vec_str);
            var_name = getArraySubscriptExprString(ASERE);
        } else if(const MemberExpr *MERE = dyn_cast<MemberExpr>(RE)){
            var_name = getMemberExprString(MERE);
        } else if(const BinaryOperator * RBO = dyn_cast<BinaryOperator>(RE)){
            getBinaryStringVector(RBO, vec_str);
        } else if(const UnaryOperator *CUO  = dyn_cast<UnaryOperator>(RE)) {
            getUnaryOperatorVector(CUO, vec_str, arr_str);
        } else if(const CXXConstructExpr * CCERE = dyn_cast<CXXConstructExpr>(RE)) {
            // CCERE;
        } else {}

        std::size_t k = 0;
        if(var_name != "") {
            for(k = 0; k < vec_str.size(); k++) {
                if(vec_str[k]==var_name){
                    break;
                }
            }
            if(k == vec_str.size()) {
                vec_str.push_back(var_name);
            }
        }
    }
}

void getUnaryOperatorVector(const UnaryOperator *UO,
                                std::vector<std::string> &vec_str,
                                std::string &arr_str) {
    if(UO->getSubExpr()) {
        const Expr *RE = UO->getSubExpr()->IgnoreCasts()->IgnoreParenImpCasts();
        std::string var_name = "";
        if(const DeclRefExpr *RDRE = dyn_cast<DeclRefExpr>(RE)){
            if(RDRE->getType()->isArrayType() || RDRE->getType()->isPointerType()) {
                arr_str = getDeclRefExprString(RDRE);
                var_name = getDeclRefExprString(RDRE);
            } else {
                var_name = getDeclRefExprString(RDRE);
            }
        } else if(const ArraySubscriptExpr *ASERE = dyn_cast<ArraySubscriptExpr>(RE)){
            getArraySubscriptExprStringVector(ASERE, vec_str);
            var_name = getArraySubscriptExprString(ASERE);
        } else if(const MemberExpr *MERE = dyn_cast<MemberExpr>(RE)){
            var_name = getMemberExprString(MERE);
            getMemberExprVector(MERE, vec_str);
        } else if(const BinaryOperator * RBO = dyn_cast<BinaryOperator>(RE)){
            getBinaryStringVector(RBO, vec_str);
        } else if(const UnaryOperator *CUO  = dyn_cast<UnaryOperator>(RE)) {
            getUnaryOperatorVector(CUO, vec_str, arr_str);
        } else if(const CXXConstructExpr * CCERE = dyn_cast<CXXConstructExpr>(RE)) {
            // CCERE;
        } else {}

        std::size_t k = 0;
        if(var_name != "") {
            for(k = 0; k < vec_str.size(); k++) {
                if(vec_str[k]==var_name){
                    break;
                }
            }
            if(k == vec_str.size()) {
                vec_str.push_back(var_name);
            }
        }
    }
}

void getMemberExprVector(const MemberExpr *ME, std::vector<std::string> &vec_str) {
    std::string var_name = "", var_name2 = "";
    std::size_t i, j;
    std::string temp_name;
    var_name2 = ME->getMemberDecl()->getDeclName().getAsString();
    const Expr *temp_expr = ME->getBase()->IgnoreCasts()->IgnoreParenImpCasts();
    if(const DeclRefExpr *LDRE = dyn_cast<DeclRefExpr>(temp_expr)){
        var_name = getDeclRefExprString(LDRE);
    } else if(const ArraySubscriptExpr *LASE = dyn_cast<ArraySubscriptExpr>(temp_expr)) {
        var_name = getArraySubscriptExprString(LASE);
        getArraySubscriptExprStringVector(LASE, vec_str);
    } else if(const MemberExpr *LME = dyn_cast<MemberExpr>(temp_expr)) {
        getMemberExprVector(LME, vec_str);
    } else if(const BinaryOperator *LBO = dyn_cast<BinaryOperator>(temp_expr)) {
        getBinaryStringVector(LBO, vec_str);
    } else if(const UnaryOperator *LUO = dyn_cast<UnaryOperator>(temp_expr)) {
        getUnaryOperatorVector(LUO, vec_str, temp_name);
    } else {}

    if(var_name != "") {
        for(i = 0; i < vec_str.size(); i++) {
            if(vec_str[i]==var_name){
                break;
            }
        }
        if(i == vec_str.size()) {
            vec_str.push_back(var_name);
        }
    }
    if(var_name2 != "") {
        for(i = 0; i < vec_str.size(); i++) {
            if(vec_str[i]==var_name2){
                break;
            }
        }
        if(i == vec_str.size()) {
            vec_str.push_back(var_name2);
        }
    }
}

void getExprVector(const Expr *E, std::vector<std::string> &vec_str) {
    string var_name;
    if(const DeclRefExpr *RDRE = dyn_cast<DeclRefExpr>(E)){
        var_name = getDeclRefExprString(RDRE);
        vec_str.push_back(var_name);
    } else if(const ArraySubscriptExpr *ASERE = dyn_cast<ArraySubscriptExpr>(E)){
        getArraySubscriptExprStringVector(ASERE, vec_str);
    } else if(const MemberExpr *MERE = dyn_cast<MemberExpr>(E)){
        getMemberExprVector(MERE, vec_str);
    } else if(const BinaryOperator * RBO = dyn_cast<BinaryOperator>(E)){
        getBinaryStringVector(RBO, vec_str);
    } else if(const CallExpr * CE = dyn_cast<CallExpr>(E)) {
        getCallExprStringVector(CE, vec_str, var_name);
    } else {}
}


void getExprArrayLenthMap(const Expr *E, map<string, string> & array_lenth_map) {
    string var_name;
    if(const DeclRefExpr *RDRE = dyn_cast<DeclRefExpr>(E)){
        ;
    } else if(const ArraySubscriptExpr *ASERE = dyn_cast<ArraySubscriptExpr>(E)){
        if(const DeclRefExpr *DRE2 = dyn_cast<DeclRefExpr>(ASERE->getBase()->IgnoreCasts()->IgnoreParenImpCasts())) {
            if(const ConstantArrayType *CAT= dyn_cast<ConstantArrayType>(DRE2->getDecl()->getType())) {
                string decl_string = getDeclRefExprString(DRE2);
                while (true) {
                    array_lenth_map[decl_string] = CAT->getSize().toString(10, true);
                    decl_string = decl_string + "++" + CAT->getSize().toString(10, true);
                    if(const ConstantArrayType *CAT2 = dyn_cast<ConstantArrayType>(CAT->getElementType())) {
                        CAT = CAT2;
                    } else {
                        break;
                    }
                }
            }
        } else if(const MemberExpr *DRE2 = dyn_cast<MemberExpr>(ASERE->getBase()->IgnoreCasts()->IgnoreParenImpCasts())) {
            if(const ConstantArrayType *CAT= dyn_cast<ConstantArrayType>(DRE2->getMemberDecl()->getType())) {
                array_lenth_map[getMemberExprString(DRE2)] = CAT->getSize().toString(10, true);
            }
            getExprArrayLenthMap(DRE2->getBase()->IgnoreCasts()->IgnoreParenImpCasts(), array_lenth_map);
        }  else if(const ArraySubscriptExpr *ASERE2 = dyn_cast<ArraySubscriptExpr>(ASERE->getBase()->IgnoreCasts()->IgnoreParenImpCasts())) {
            getExprArrayLenthMap(ASERE2, array_lenth_map);
        } else {
            ;
        }
    } else if(const MemberExpr *MERE = dyn_cast<MemberExpr>(E)){
        getExprArrayLenthMap(MERE->getBase()->IgnoreCasts()->IgnoreParenImpCasts(), array_lenth_map);
    } else if(const BinaryOperator * RBO = dyn_cast<BinaryOperator>(E)){
        getExprArrayLenthMap(RBO->getRHS()->IgnoreCasts()->IgnoreParenImpCasts(), array_lenth_map);
        getExprArrayLenthMap(RBO->getLHS()->IgnoreCasts()->IgnoreParenImpCasts(), array_lenth_map);
    } else if(const CallExpr * CE = dyn_cast<CallExpr>(E)) {
        unsigned NumArgs = CE->getNumArgs();
        for (unsigned i = 0; i < NumArgs; i++){
            const Expr *RE = CE->getArg(i)->IgnoreCasts()->IgnoreParenImpCasts();
            getExprArrayLenthMap(RE, array_lenth_map);
        }
    } else if(const UnaryOperator *UO = dyn_cast<UnaryOperator>(E)) {
        getExprArrayLenthMap(UO->getSubExpr()->IgnoreCasts()->IgnoreParenImpCasts(), array_lenth_map);
    }  else {}
}

bool getExprUnoinSign(const Expr *E) {
    if(!E)
        return false;
    if(const DeclRefExpr *DRE = dyn_cast<DeclRefExpr>(E)) {
        return false;
    } else if(const ArraySubscriptExpr *ASE = dyn_cast<ArraySubscriptExpr>(E)) {
        if(!ASE->getBase())
            return false;
        return getExprUnoinSign(ASE->getBase()->IgnoreCasts()->IgnoreParenImpCasts());
    } else if(const MemberExpr *ME = dyn_cast<MemberExpr>(E)) {
        if (!ME->getBase())
            return false;
        if(StartWith(ME->getBase()->getType().getAsString(), "union ")) {
            return true;
        } else {
            return getExprUnoinSign(ME->getBase()->IgnoreCasts()->IgnoreParenImpCasts());
        }
    }
    return false;
}

string& replace(string& s, const string& from, const string& to) {
    if(!from.empty())
        for(size_t pos = 0; (pos = s.find(from, pos)) != string::npos; pos += to.size())
            s.replace(pos, from.size(), to);
    return s;
}

string reverseVariableStringToNormal(string str) {
    str = replace(str, "..", ".");
    string str2 = "";
    if(str[str.size()-1] == '+') {
        return str;
    }
    int sign = 0;
    for(size_t index=0; index<str.size(); index++) {
        if(str[index] == '+' && str[index+1] == '+'){
            //增加对多维数组的支持
            if(sign == 1)
            {
                str2 += ']';
                sign -= 1;
            }
            str2 += '[';
            sign += 1;
            index += 2;
        } else if(str[index] == '.' && sign==1) {
            str2 += ']';
            sign -= 1;
        }
        {
            str2 += str[index];
        }
    }
    if(sign==1) {
        str2 += ']';
    }

    return str2;
}

string reverseKeyStringToNormal(string str) {
    return replace(str, "arrayindex", "数组下标");
}

} // namespace bz12
} // namespace tidy
} // namespace clang