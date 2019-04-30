// #include <jsoncpp/json/json.h>
// #include <string>
// #include <iostream>
// #include <chrono>
// using namespace std;

// struct MyStruct
// {
// 	int i;
// };

// int main()
// {
// 	Json::Value root;  // ��ʾ���� json ����
// 	root["platenumber"] = Json::Value("value_string");
// 	root["platetype"] = Json::Value(0);
// 	root["snopshotplaceid"] = Json::Value("12345678");
// 	root["snopshottime"] = Json::Value("20171222");
// 	root["platenumber"] = Json::Value("123456");
// 	root["imgpath"] = Json::Value("D:/TEST.JPG");
// 	root["extFlag"] = Json::Value("EXTFLAG");
// 	root["taskid"] = Json::Value("TASKID");

// 	Json::ValueType type = root.type();                       // ��� root �����ͣ��˴�Ϊ objectValue ���͡�

// 	Json::StyledWriter styled_writer;

// 	cout << styled_writer.write(root) << endl;

// 	string aass = styled_writer.write(root);
// 	cout << aass << endl;
// 	Json::Reader read;

// 	Json::Value roota;
// 	std::string jText = "{\"praenomen\":\"Gaius\",\"nomen\":\"Julius\",\"cognomen\":\"Caezar\","  
// 		"\"born\":-100,\"died\":-44}";

// 	std::chrono::time_point<std::chrono::high_resolution_clock> Begin = std::chrono::high_resolution_clock::now();
// 	for (int i = 0; i != 10000; i++)
// 	{
// 		read.parse(jText, roota);
// 	}
// 	auto Diff = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - Begin);
// 	cout << Diff.count() << endl;

// 	return 0;
// }