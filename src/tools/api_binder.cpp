#include <defines.h>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>

struct function_info
{
	std::string type;
	std::string name;
	std::string args;

	std::string GetMacros()
	{
		std::string result;
		result += "#define " + name + "Signature(name) " + type + " name" + args + "\n";
		result += "typedef " + name + "Signature(_" + name + ");\n";
		result += "#define pfn" + name + " _" + name + "*\n";
		return result;
	}

	std::string GetPfnType()
	{
		return "pfn" + name;
	}

	std::string GetPfnMember()
	{
		return GetPfnType() + " " + name + ";";
	}

	std::string GetStub()
	{
		std::string result;
		result += type + " " + name + "Stub" + args + "\n";
		result += "{\n";
		result += "    LOG_DEBUG(\"Called Stub Function '%s'\", __FUNCTION__);\n";

		if (type == "bool")
			result += "    return false;\n";
		
		result += "}\n";
		return result;
	}

	std::string GetFunctionAssignment()
	{
		std::string result = "\tapi." + name + " = (" + GetPfnType() + ")PlatformGetLibraryFunction(&library, \"" + name + "\");\n";
		result += "\tif (!api." + name + ") " + "api." + name + " = " + name + "Stub;\n";
		return result;
	}
	
	std::string GetFunctionAssignmentStub()
	{
		std::string result = "\tapi." + name + " = " + name + "Stub;\n";
		return result;
	}
};

bool GenerateFunctionBindings(const char* cppFile, const char* apiName, const char* outDir)
{
	std::ifstream file(cppFile);
	if (!file.good())
	{
		file.close();
		return false;
	}

	std::vector<function_info> functions;
	functions.reserve(10);
	std::string line;
	while (std::getline(file, line))
	{
		static std::string dllexportToken = " HY3D_API ";
		size_t dllexportTokenPos = line.find(dllexportToken);
		if (dllexportTokenPos != std::string::npos)
		{
			function_info func = {};

			line = line.substr(dllexportTokenPos + dllexportToken.length());
			line = line.substr(line.find_first_not_of(' '));

			size_t firstSpacePos = line.find_first_of(' ');
			func.type = line.substr(0, firstSpacePos);
			line = line.substr(line.find_first_not_of(' ', firstSpacePos + 1));

			size_t argStartPos = line.find_first_of('(');
			func.name = line.substr(0, argStartPos);
			line = line.substr(line.find_first_not_of(' ', argStartPos));

			func.args = line.substr(0, line.find_first_of(')') + 1);

			functions.push_back(func);
		}
	}
	file.close();

	std::string hFilepath = std::string(outDir) + "/" + std::string(apiName) + "_auto.h";

	std::ofstream autofileH(hFilepath);
	if (!autofileH.good())
	{
		autofileH.close();
		return false;
	}

	for (function_info& func : functions)
	{
		autofileH <<  func.GetMacros() << '\n';
	}
	autofileH << "\n";

	autofileH << "struct " << apiName << "\n";
	autofileH << "{\n";
	for (function_info& func : functions)
	{
		autofileH << "    " << func.GetPfnMember() << '\n';
	}
	autofileH << "};\n\n";
	autofileH.close();
	
	std::string cppFilepath = std::string(outDir) + "/" + std::string(apiName) + "_auto.cpp";
	std::ofstream autofileCPP(cppFilepath);
	if (!autofileCPP.good())
	{
		autofileCPP.close();
		return false;
	}

	for (function_info& func : functions)
	{
		autofileCPP <<  func.GetStub() << '\n';
	}
	autofileCPP << "\n";

	autofileCPP << apiName << " " << apiName << "_get(dynamic_library& library)\n";
	autofileCPP << "{\n";
	autofileCPP << "\t" << apiName << " api = {};\n\n";
	for (function_info& func : functions)
	{
		autofileCPP << func.GetFunctionAssignment() << '\n';
	}
	autofileCPP << "\treturn api;\n";
	autofileCPP << "}\n\n"; 
	
	autofileCPP << apiName << " " << apiName << "_get_stubs()\n";
	autofileCPP << "{\n";
	autofileCPP << "\t" << apiName << " api = {};\n\n";
	for (function_info& func : functions)
	{
		autofileCPP << func.GetFunctionAssignmentStub();
	}
	autofileCPP << "\treturn api;\n";
	autofileCPP << "}\n\n"; 

	autofileCPP.close();

	return true;
}

// <cpp file> <api name> <out dir>
int main(int argc, char* argv[])
{
	int result = 0;

	std::cout << "------------------------------------------------" << std::endl;
	std::cout << "GENERATING API BINDINGS" << std::endl;
	
	if (argc != 4)
	{
		std::cout << argc;
		std::cout << "Argumments missing. Usage: api_binder <cpp file> <api name> <out dir>\n";
		result = 1;
	}
	else
	{
		std::cout << "Parsing " << argv[1] << std::endl;
		std::cout << "Creating " << argv[2] << " in Directory " << argv[3] << std::endl;

		if (!GenerateFunctionBindings(argv[1], argv[2], argv[3]))
		{
			result = 2;
		}
	}

	std::cout << "------------------------------------------------" << std::endl;

	return result;
}