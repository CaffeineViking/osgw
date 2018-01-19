#include <osgw/shader.hh>

#include <regex>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iterator>
#include <stdexcept>

namespace osgw {
    Shader::Shader(const std::string& path, Type type)
                   : type { type } {
        std::string source;
        source = load(path);
        source = preprocess(source, path);

        handle = glCreateShader(static_cast<GLenum>(type));

        const GLint source_length = source.length();
        const GLchar* const source_pointer { source.c_str() };

        glShaderSource(handle, 1, &source_pointer, &source_length);
        glCompileShader(handle);

        GLint compile_success;
        glGetShaderiv(handle, GL_COMPILE_STATUS, &compile_success);
        if (!compile_success) {
            GLint log_length;
            glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &log_length);

            GLchar* log { new GLchar[log_length] }; // Will leak >:).
            glGetShaderInfoLog(handle, log_length, &log_length, log);
            std::cerr << source << std::endl;
            throw std::runtime_error { log };
        }
    }

    Shader::~Shader() noexcept {
        glDeleteShader(handle);
    }

    std::string Shader::load(const std::string& path) const {
        std::ifstream source_stream { path };
        if (!source_stream) throw std::runtime_error { "Failed to load '" + path + "'!" };
        std::string source { std::istreambuf_iterator<char> { source_stream },
                             std::istreambuf_iterator<char> {   } };
        return source;
    }

    std::string Shader::preprocess(const std::string& source, const std::string& path) const {
        std::string preprocessed_source;
        std::istringstream source_stream { source };

        if (deps.find(path) == deps.end()) deps.insert(path);
        else return ""; // Already have this dependency here.

        std::string source_line;
        std::regex pragma_regex { R"(#pragma\sinclude\(".*"\)\s*)" };
        std::regex version_regex { R"(#version\s\d{3}\s*)" };
        std::regex include_regex { R"(\(".*"\))" };

        std::smatch version_match;
        std::getline(source_stream, source_line);
        if (std::regex_match(source_line, version_match, version_regex)) {
            std::string source_version { version_match.str().substr(9, 12) };
            if (version == "???") {
                version = source_version;
                preprocessed_source += "#version " + version + "\n";
            } else if (version != source_version)
                throw std::runtime_error { "Non-matching version" };
        }

        while (std::getline(source_stream, source_line)) {
            if (std::regex_match(source_line, pragma_regex)) {
                std::smatch include_match;
                std::regex_search(source_line, include_match, include_regex);
                std::string include_path { include_match.str() };
                include_path = include_path.substr(2, include_path.size() - 4);

                std::string relative_path;
                auto relative_path_position = path.find_last_of('/');
                if (relative_path_position != std::string::npos)
                    relative_path = path.substr(0, relative_path_position + 1);
                relative_path += include_path;

                std::string dependency_source { load(relative_path) };
                std::string preprocessed_dependency { preprocess(dependency_source, relative_path) };
                preprocessed_source += preprocessed_dependency;
                continue;
            }

            preprocessed_source += source_line + "\n";
        }

        return preprocessed_source;
    }
}
