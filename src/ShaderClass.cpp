#include "ShaderClass.h"

std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return contents;
	}
	throw std::runtime_error("Failed to open file: " + std::string(filename));
}

Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
	std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;

	std::string defineStr = "\n#define USE_GEOMETRY\n";

    try
    {
        vertexCode = get_file_contents(vertexPath);
        fragmentCode = get_file_contents(fragmentPath);
		if (geometryPath != nullptr)
		{
        	geometryCode = get_file_contents(geometryPath);
			size_t pos = vertexCode.find('\n');
			vertexCode.insert(pos + 1, defineStr);
		}
    }
    catch (const std::exception& e)
    {
        std::cerr << "Shader file loading error: " << e.what() << std::endl;
    }
	
	GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vertexCode.c_str());
	compileErrors(vertexShader, "VERTEX");
	
	GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentCode.c_str());
	compileErrors(fragmentShader, "FRAGMENT");

	GLuint geometryShader = 0;
	if (!geometryCode.empty())
	{
		geometryShader = CompileShader(GL_GEOMETRY_SHADER, geometryCode.c_str());
		compileErrors(geometryShader, "GEOMETRY");
	}

	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	if (geometryShader != 0)
		glAttachShader(ID, geometryShader);
	glLinkProgram(ID);
	compileErrors(ID, "PROGRAM");

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	if (geometryShader != 0)
		glDeleteShader(geometryShader);
}

GLuint Shader::CompileShader(GLuint type, const char* source)
{
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);
	return shader;
}

void Shader::Activate()
{
	glUseProgram(ID);
}

void Shader::Delete()
{
	glDeleteProgram(ID);
}

void Shader::compileErrors(unsigned int shader, const char *type)
{
	// Stores status of compilation
	GLint hasCompiled;
	// Character array to store error message in
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_COMPILATION_ERROR for:" << type << "\n"
					  << infoLog << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_LINKING_ERROR for:" << type << "\n"
					  << infoLog << std::endl;
		}
	}
}