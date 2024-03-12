#include <set>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "rapidjson/include/rapidjson/document.h"
#include "../ClangTidyCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/Expr.h"

using namespace std;
namespace clang {
namespace tidy {
namespace bz12 {

    string GetExecutablePath(const char *Argv0);
    string GetCheckerConfigFilePath(const char *argv0);
    string read_nested_variable_check_mode();
    void read_custom_keyword_levelset_map (map<string, set<string>> &keyword_levelset_map);
    void read_default_keyword_levelset_map (map<string, set<string>> &keyword_levelset_map);
    void read_negative_string_vector(vector<string> & negative_string_vector);
    void read_default_string_set (set<string> & default_string_set);
    void read_default_string_level_map (map<string, set<string>> & default_string_level_map);
    void read_custom_string_set (set<string> & custom_string_set);
    void read_custom_string_level_map (map<string, set<string>> & custom_string_level_map);
    bool read_custom_case_sensitive ();
    void read_subname_map(map<string, string> &sub_name_group_map, 
                            map<string, string> &sub_name_level_map, 
                            map<string, map<string, string>> & group_map);
    void read_mutex_level_map (map<string, string> &mutex_level_map);
    void read_expected_value_map(map<string, vector<double>> &expected_value_map);
    void processJsonMember(const rapidjson::Value &memberValue, map<vector<string>, vector<double>> &expected_value_map, string varName);

    void read_struct_alignment(int &alignment);
    void read_double_assign_negative_vector(vector<string> & double_assign_arr_negative_vec);

    bool pattern_matchs(std::string reg_string, std::string pattern="[a-zA-Z0-9*]*[*]+[a-zA-Z0-9*]*");
    bool fuzzy_matchs(std::string fuzz_string, int start_pos, int diff_count, std::string pattern, int & start_pos2);
    string get_default_string_level(string key_word, map<string,set<string>> default_string_level_map);
    string get_custom_string_level(string key_word, map<string,set<string>> custom_string_level_map);
    string to_lower_string(string str);

    bool case_insensitive_find_in_vector(string key_word, vector<string> vec_str, size_t &index);
    bool case_insensitive_find_in_set(string key_word, set<string> set_str, string set_key_word);
    bool case_insensitive_find_in_negative_vector(string key_word, vector<string> vec_str, size_t &index);
    void transform_default_string_set_to_list(list<string> & default_string_list_sorted_by_lenth, set<string> default_string_set);
    void transform_custom_string_set_to_list(list<string> & custom_string_list_sorted_by_lenth, set<string> custom_string_set);

    bool analysisNameSimilarity(
        list<string> default_string_list_sorted_by_lenth, list<string> custom_string_list_sorted_by_lenth, 
        string name1, string name2, string & sub_name1, string & sub_name2, int & start_pos1, int & start_pos2);

    bool analysisNameSimilarityV2(
        list<string> default_string_list_sorted_by_lenth, list<string> custom_string_list_sorted_by_lenth, 
        string name1, string name2, vector<string> & vec_sub_name1, vector<string> & vec_sub_name2);

    bool analysisArraySubNameSimilarity(
        list<string> default_string_list_sorted_by_lenth, list<string> custom_string_list_sorted_by_lenth, 
        string name1, string name2, string & sub_name1, string & sub_name2, int & start_pos1, int & start_pos2);

    string get_group_name_prefix(string name1, string name2, string sub_name1, string sub_name2);

    bool StartWith(std::string s1, std::string s2);
    string& replace(string& s, const string& from, const string& to);
    string reverseVariableStringToNormal(string str);
    string reverseKeyStringToNormal(string str);

    std::string getInterLiteralExprString(const BinaryOperator *BO);
    std::string getArraySubscriptExprString(const ArraySubscriptExpr * ASE);
    std::string getMemberExprString(const MemberExpr * ME);
    std::string getDeclRefExprString(const DeclRefExpr * DRE);
    void getBinaryStringVector(const BinaryOperator *BO, std::vector<std::string> &vec_str);
    void getArraySubscriptExprStringVector(const ArraySubscriptExpr *ASE, std::vector<std::string> &vec_str);
    void getCallExprStringVector(const CallExpr *CE, std::vector<std::string> &vec_str, std::string &arr_str);
    void getUnaryOperatorVector(const UnaryOperator *UO, std::vector<std::string> &vec_str, std::string &arr_str);
    void getMemberExprVector(const MemberExpr *ME, std::vector<std::string> &vec_str);
    void getExprVector(const Expr *E, std::vector<std::string> &vec_str);

    void getExprArrayLenthMap(const Expr *E, std::map<std::string, std::string> & array_lenth_map);

    bool getExprUnoinSign(const Expr *E);

} // namespace bz12
} // namespace tidy
} // namespace clang