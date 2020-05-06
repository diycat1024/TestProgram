#pragma once
#include "../acl_cpp_define.hpp"
#include <map>
#include <vector>
#include "../stdlib/string.hpp"
#include "redis_command.hpp"

#if !defined(ACL_CLIENT_ONLY) && !defined(ACL_REDIS_DISABLE)

namespace acl
{

class redis_client;

/**
 * redis Hash(��ϣ��) �࣬�����ʵ�ֵ���Ҫ���
 * redis Hash class, include commands as below:
 * HDEL/HEXISTS/HGET/HGETALL/HINCRBY/HINCRBYFLOAT/HKEYS/HLEN/HMGET/HMSET
 * HSET/HSETNX/HVALS/HSCAN
 */
class ACL_CPP_API redis_hash : virtual public redis_command
{
public:
	/**
	 * see redis_command::redis_command()
	 */
	redis_hash(void);

	/**
	 * see redis_command::redis_command(redis_client*)
	 */
	redis_hash(redis_client* conn);

	/**
	 * see redis_command::redis_command(redis_client_cluster*, size_t)
	 */
	redis_hash(redis_client_cluster* cluster, size_t max_conns = 0);

	virtual ~redis_hash(void);

	/////////////////////////////////////////////////////////////////////

	/**
	 * �����"��-ֵ"�������� KEY ��Ӧ�Ĺ�ϣ����
	 * HMSET: set the key's multiple fileds in redis-server
	 * @param key {const char*} ��ϣ�� key ֵ
	 *  the hash key for Hash class
	 * @param attrs {const std::map<acl::string, ...>&} the fileds in map
	 * @return {bool} �����Ƿ�ɹ�
	 *  if successful for HMSET command
	 */
	bool hmset(const char* key, const std::map<string, string>& attrs);
	bool hmset(const char* key, const std::map<string, const char*>& attrs);
	bool hmset(const char* key, const std::vector<string>& names,
		const std::vector<string>& values);
	bool hmset(const char* key, const std::vector<const char*>& names,
		const std::vector<const char*>& values);
	bool hmset(const char* key, const char* names[], const char* values[],
		size_t argc);
	bool hmset(const char* key, const char* names[], const size_t names_len[],
		const char* values[], const size_t values_len[], size_t argc);

	/////////////////////////////////////////////////////////////////////

	/**
	 * ���� KEY ֵ�����"��-ֵ"�Դӹ�ϣ����ȡ��
	 * get the values associated with the specified fields
	 * in the hash stored at key
	 * @param key {const char*} ��ϣ�� key ֵ
	 *  the hash key
	 * @param names ��Ӧ key ����ֵ��
	 *  the given hash fileds
	 * @param result {std::vector<acl::string>*} ���ö���ָ��ǿ�ʱ�洢��ѯ�����
	 *  ����ò���Ϊ NULL ʱ�������ͨ������ result_/get_ �������
	 *  store the result of the given hash files if not NULL.
	 *  If NULL, the base class's method like result_/get can be used
	 *  to get the values
	 * @return {bool} �����Ƿ�ɹ��������ɹ������ͨ��������һ�ַ�ʽ������ݣ�
	 *  if successul, one of below ways can be used to get the result:
	 *
	 *  1���ڵ��÷����д���ǿյĴ洢�������ĵ�ַ
	 *     input the no-NULL result parameter when call hmget, when
	 *     success, the result will store the values of the given fileds
	 *
	 *  2�����෽�� result_value ���ָ���±��Ԫ������
	 *     call redis_command::result_value with the specified subscript
	 *
	 *  3�����෽�� result_child ���ָ���±��Ԫ�ض���(redis_result����Ȼ����ͨ��
	 *     redis_result::argv_to_string �������Ԫ������
	 *     call redis_command::result_child with specified subscript to
	 *     get redis_result object, then call redis_result::argv_to_string
	 *     with above result to get the values of the give fileds
	 *
	 *  4�����෽�� get_result ����ȡ���ܽ�������� redis_result��Ȼ����ͨ��
	 *     redis_result::get_child ���һ��Ԫ�ض���Ȼ����ͨ����ʽ 2 ��ָ��
	 *     �ķ�����ø�Ԫ�ص�����
	 *     call redis_command::get_result with the specified subscript to
	 *     get redis_result object, and use redis_result::get_child to
	 *     get one result object, then call redis_result::argv_to_string
	 *     to get the value of one filed.
	 *
	 *  5�����෽�� get_children ��ý��Ԫ�����������ͨ�� redis_result ��
	 *     �ķ��� argv_to_string ��ÿһ��Ԫ�ض����л��Ԫ������
	 *     use redis_command::get_children to get the redis_result array,
	 *     then use redis_result::argv_to_string to get every value of
	 *     the given fileds
	 */
	bool hmget(const char* key, const std::vector<string>& names,
		std::vector<string>* result = NULL);
	bool hmget(const char* key, const std::vector<const char*>& names,
		std::vector<string>* result = NULL);

	bool hmget(const char* key, const char* names[], size_t argc,
		std::vector<string>* result = NULL);
	bool hmget(const char* key, const char* names[], const size_t lens[],
		size_t argc, std::vector<string>* result = NULL);

	/////////////////////////////////////////////////////////////////////

	/**
	 * ���� key ������ĳ�����ֶε�ֵ
	 * set one field's value in the hash stored at key.
	 * @param key {const char*} key ��ֵ
	 *  the hash key
	 * @param name {const char*} key �����������
	 *  the filed name of the hash key
	 * @param value {const char*} key �������ֵ
	 *  the filed value of the hash key
	 * @return {int} ����ֵ���壺
	 *  1 -- ��ʾ�����ӵ����ֶ����ӳɹ�
	 *  0 -- ��ʾ�����Ѿ����ڵ����ֶγɹ�
	 * -1 -- ��ʾ������� key ����ǹ�ϣ�����ӽ���ֹ�޸�
	 *  return int value as below:
	 *  1 -- this is a new filed and set ok
	 *  0 -- thie is a old filed and set ok
	 * -1 -- error happend or the key is not a Hash type
	 */
	int hset(const char* key, const char* name, const char* value);
	int hset(const char* key, const char* name,
		const char* value, size_t value_len);
	int hset(const char* key, const char* name, size_t name_len,
		const char* value, size_t value_len);

	/**
	 * ���ҽ��� key �����е�ĳ�����ֶβ�����ʱ�Ÿ��¸����ֶ�ֵ
	 * set one new field of one key in hash only when the filed isn't
	 * existing.
	 * @param key {const char*} key ��ֵ
	 *  the hash key
	 * @param name {const char*} key �����������
	 *  the field name
	 * @param value {const char*} key �������ֵ
	 *��the field value
	 * @return {int} ����ֵ���壺
	 *  1 -- ��ʾ�����ӵ����ֶ����ӳɹ�
	 *  0 -- �����ֶδ�����δ������и���
	 * -1 -- ��ʾ������� key ����ǹ�ϣ�����ӽ���ֹ�޸�
	 *
	 *  return int value as below:
	 *  1 -- this is a new filed and set ok
	 *  0 -- thie is a old filed and not set
	 * -1 -- error happend or the key is not a Hash type
	 */
	int hsetnx(const char* key, const char* name, const char* value);
	int hsetnx(const char* key, const char* name,
		const char* value, size_t value_len);
	int hsetnx(const char* key, const char* name, size_t name_len,
		const char* value, size_t value_len);

	/**
	 * �� redis ��ϣ���л�ȡĳ�� key �����ĳ�����ֵ
	 * get the value assosiated with field in the hash stored at key
	 * @param key {const char*} key ��ֵ
	 *  the hash key
	 * @param name {const char*} key ��������ֶ�����
	 *  the field's name
	 * @param result {acl::string&} �洢��ѯ���ֵ(�ڲ��Ը� string ��������׷��)
	 *  store the value result of the given field
	 * @return {bool} ����ֵ���壺
	 *  true -- �����ɹ�����resultΪ��ʱ��ʾ KEY ���ֶ��򲻴���
	 *          get the value associated with field; if result is empty then
	 *          the key or the name field doesn't exist
	 *  false -- ���ֶβ����ڻ����ʧ�ܻ�� key ����ǹ�ϣ����
	 *           the field not exists, or error happened,
	 *           or the key isn't a hash key
	 */
	bool hget(const char* key, const char* name, string& result);
	bool hget(const char* key, const char* name,
		size_t name_len, string& result);

	/**
	 * �� redis ��ϣ���л�ȡĳ�� key ������������ֶε�ֵ
	 * get all the fields and values in hash stored at key
	 * @param key {const char*} key ��ֵ
	 *  the hash key
	 * @param result {std::map<string, string>&} �洢���ֶ���-ֵ��ѯ�����
	 *  store the result of all the fileds and values
	 * @return {bool} �����Ƿ�ɹ������壺
	 *  if ok, show below:
	 *  true -- �����ɹ��������򲻴���ʱҲ���سɹ�����Ҫ��� result �����Ƿ�仯��
	 *          �������ͨ����� result.size() �ı仯�������Ƿ��ѯ�����
	 *          successful if the key is a hash key or the key not exists
	 *  false -- ����ʧ�ܻ�� key ����ǹ�ϣ����
	 *           error happened or the key isn't a hash key
	 */
	bool hgetall(const char* key, std::map<string, string>& result);
	bool hgetall(const char* key, std::vector<string>& names,
		std::vector<string>& values);
	bool hgetall(const char* key, std::vector<const char*>& names,
		std::vector<const char*>& values);

	/**
	 * �� redis ��ϣ����ɾ��ĳ�� key �����ĳЩ���ֶ�
	 * remove one or more fields from hash stored at key
	 * @param key {const char*} key ��ֵ
	 *  the hash key
	 * @param first_name {const char*} ��һ�����ֶ��������һ���ֶα����� NULL
	 *  the first field of the fields list, the last field must be NULL
	 *  indicating the end of vary parameters
	 * @return {int} �ɹ�ɾ�������ֶθ��������� -1 ��ʾ������� key ����ǹ�ϣ����
	 *  return the number of fields be removed successfully, or -1 when
	 *  error happened or operating on a no hash key
	 */
	int hdel(const char* key, const char* first_name);
	int hdel(const char* key, const char* names[], size_t argc);
	int hdel(const char* key, const char* names[],
		const size_t names_len[], size_t argc);
	int hdel(const char* key, const std::vector<string>& names);
	int hdel(const char* key, const std::vector<const char*>& names);
	int hdel_fields(const char* key, const char* names[], size_t argc);
	int hdel_fields(const char* key, const char* names[],
		const size_t names_len[], size_t argc);
	int hdel_fields(const char* key, const std::vector<string>& names);
	int hdel_fields(const char* key, const std::vector<const char*>& names);
	int hdel_fields(const char* key, const char* first_name, ...);

	/**
	 * ��ĳ�� key �����е�ĳ�����ֶ�Ϊ����ʱ��������мӼ�����
	 * inc(+n) or dec(-n) on a integer filed in hash stored at key
	 * @param key {const char*} key ��ֵ
	 *  the hash key
	 * @param name {const char*} key ��������ֶ�����
	 *  the filed name of integer type
	 * @param inc {long long int} ���ӵ�ֵ������Ϊ��ֵ
	 *  the integer value to be inc or dec on the field's value
	 * @param result {long long int*} �� NULL ʱ�洢���ֵ
	 *  store the result if non-NULL
	 * @return {bool} �����Ƿ�ɹ��������� false ʱ����������� key ����ǹ�ϣ
	 *  ���������ֶη���������
	 *  if successful: false when error, not a hash, or the field isn't
	 *  integer type
	 */
	bool hincrby(const char* key, const char* name,
		long long int inc, long long int* result = NULL);

	/**
	 * ��ĳ�� key �����е�ĳ�����ֶ�Ϊ������ʱ��������мӼ�����
	 * inc(+n) or dec(-n) on a float filed in hash stored at key
	 * @param key {const char*} key ��ֵ
	 *  the hash key
	 * @param name {const char*} key ��������ֶ�����
	 *  the filed name of float type
	 * @param inc {double} ���ӵ�ֵ������Ϊ��ֵ
	 *  the float value to be inc or dec on the field's value
	 * @param result {double*} �� NULL ʱ�洢���ֵ
	 *  store the result if non-NULL
	 * @return {bool} �����Ƿ�ɹ��������� false ʱ����������� key ����ǹ�ϣ
	 *  ���������ֶηǸ���������
	 *  if successful: false when error, not a hash, or the field isn't
	 *  float type
	 */
	bool hincrbyfloat(const char* key, const char* name,
		double inc, double* result = NULL);

	/**
	 * ���� key �������������ֶ�����
	 * get all the fields in hash stored at key
	 * @param key {const char*} key ��ֵ
	 *  the hash key
	 * @param names {std::vector<string>&} �洢�� key �����������ֶ�����
	 *  store all the names of all fileds
	 * @return {bool} �����Ƿ�ɹ������� false ����������� key ����ǹ�ϣ����
	 *  return true on success, false if error happened or the
	 *  key wasn't a hash key
	 */
	bool hkeys(const char* key, std::vector<string>& names);

	/**
	 * ��� key ������ĳ�����ֶ��Ƿ����
	 * check if the field exists in hash stored at key
	 * @param key {const char*} key ��ֵ
	 *  the hash key
	 * @param name {const char*} key ��������ֶ�����
	 *  the filed's name of the key
	 * @return {bool} �����Ƿ�ɹ������� false ����������� key ����ǹ�ϣ����
	 *  ������ֶβ�����
	 *  return true on success, false if error happened or the
	 *  key wasn't a hash key
	 */
	bool hexists(const char* key, const char* name);
	bool hexists(const char* key, const char* name, size_t name_len);

	/**
	 * ���ĳ�� key �������������ֶε�����
	 * get the count of fields in hash stored at key
	 * @param key {const char*} key ��ֵ
	 *  the hash key
	 * @return {int} ����ֵ���壺
	 *  return int value as below:
	 *  -1 -- ������� key ����ǹ�ϣ����
	 *        error or not a hash key
	 *  >0 -- ���ֶ�����
	 *        the count of fields
	 *   0 -- �� key �����ڻ����ֶ�����Ϊ 0
	 *        key not exists or no fields in hash stored at key 
	 */
	int hlen(const char* key);

	/**
	 * ���ĳ�� key �е�ָ��������ݳ���
	 * Returns the string length of the value associated with field
	 * in the hash stored at key
	 * @param key {const char*} key ��ֵ
	 *  the hash key
	 * @param name {const char*} key ��������ֶ�����
	 *  the field's name
	 * @return {int} ��� key �� name �����ڣ��򷵻� 0����� key �ǹ�ϣ
	 *  ����������򷵻� -1
	 *  If the key or the field do not exist, 0 is returned; If the key is
	 *  not the hash key or error happened, -1 is returned.
	 */
	int hstrlen(const char* key, const char* name, size_t name_len);
	int hstrlen(const char* key, const char *name);
	
	/**
	 * �������ڵ�����ϣ���еļ�ֵ��
	 * scan the name and value of all fields in hash stored at key
	 * @param key {const char*} ��ϣ��ֵ
	 *  the hash key
	 * @param cursor {int} �α�ֵ����ʼ����ʱ��ֵд 0
	 *  the cursor value, which is 0 at begin
	 * @param out {std::map<acl::string>&} �洢��������ڲ���׷�ӷ�ʽ������
	 *  ����������ӽ��ö����У�Ϊ��ֹ���ܽ���������¸�����������û�����
	 *  ���ñ�����ǰ�������ö���
	 *  store scaning result in appending mode
	 * @param pattern {const char*} ƥ��ģʽ��glob ��񣬷ǿ�ʱ��Ч
	 *  match pattern, effective only on no-NULL
	 * @param count {const size_t*} �޶��Ľ�����������ǿ�ָ��ʱ��Ч
	 *  the max count of one scan process, effective only on no-NULL
	 * @return {int} ��һ���α�λ�ã��������£�
	 *  return the next cursor position, as below:
	 *   0����������
	 *     scan finish
	 *  -1: ����
	 *     some error happened
	 *  >0: �α����һ��λ�ã���ʹ�����������ж��ٽ������Ҫ��� out����Ϊ�п���Ϊ��
	 *     the next cursor postion to scan
	 */
	int hscan(const char* key, int cursor, std::map<string, string>& out,
		const char* pattern = NULL, const size_t* count = NULL);
};

} // namespace acl

#endif // !defined(ACL_CLIENT_ONLY) && !defined(ACL_REDIS_DISABLE)
