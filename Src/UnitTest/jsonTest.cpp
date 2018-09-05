/*#include <jsoncpp/json/json.h>
#include <string>
#include <iostream>
using namespace std;

void jsonTest()
{
	Json::Value root;  // 表示整个 json 对象
	root["platenumber"] = Json::Value("value_string");
	root["platetype"] = Json::Value(0);
	root["snopshotplaceid"] = Json::Value("12345678");
	root["snopshottime"] = Json::Value("20171222");
	root["platenumber"] = Json::Value("123456");
	root["imgpath"] = Json::Value("D:/TEST.JPG");
	root["extFlag"] = Json::Value("EXTFLAG");
	root["taskid"] = Json::Value("TASKID");

	Json::ValueType type = root.type();                       // 获得 root 的类型，此处为 objectValue 类型。

	Json::StyledWriter styled_writer;

	cout << styled_writer.write(root) << endl;

	string aass = styled_writer.write(root);
	cout << aass << endl;
	Json::Reader read;

	Json::Value roota;
	std::string jText = "{\"praenomen\":\"Gaius\",\"nomen\":\"Julius\",\"cognomen\":\"Caezar\","  
		"\"born\":-100,\"died\":-44}";
	if (read.parse(jText, roota))
		cout << styled_writer.write(roota) << endl;
}
*/