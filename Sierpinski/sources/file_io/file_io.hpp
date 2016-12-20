/**
 * @brief This file provides some simple functions to read/write files with
 * the *stream libraries from C++ standard
 * @example example.cpp
 * @author MTLCRBN
 * @version 2.0
 */
#ifndef FILE_IO_HPP_INCLUDED
#define FILE_IO_HPP_INCLUDED

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdint>
#include <functional>


#if defined(__GNUC__)
	#define NONNULL(...) __attribute__((nonnull (__VA_ARGS__)))
	#define UNUSED(var) __attribute__((unused)) var
#else
	#define NONNULL(...)
	#define UNUSED(var) var
#endif

#ifndef DECLSPEC
	#if defined(__WIN32__) || defined(__WINRT__)
		#define DECLSPEC __declspec(dllexport)
	#else
		#if defined(__GNUC__) && __GNUC__ >= 4
			#define DECLSPEC __attribute__((visibility("default")))
		#elif defined(__GNUC__) && __GNUC__ >= 2
			#define DECLSPEC __declspec(dllexport)
		#else
			#define DECLSPEC
		#endif
	#endif
#endif


/**
 * @brief Get the absolute name of \a nameFromRoot, using \a argv0, and storing the result
 * on \a out.
 * @throw std::string if an error occurs.
 * 
 * @param[in]  nameFromRoot Considering the root of your project tree, the relative path to get your file.
 * @param[in]  argv0        the first argument of the program (argv[0]).
 * @param[out] out          The storage place for the result.
 * @return A reference on \a out.
 */
DECLSPEC std::string& getFullNameOfFile(const std::string &nameFromRoot, const char *const argv0, std::string &out) NONNULL(2);


/**
 * @brief An input/output file wrapper class.
 */
template<typename FileType>
class File final
{
	private:
		FileType file;
	
	public:
		static const int32_t ANY_LENGTH_THAN_ZERO = -1;
		
		/**
		 * @brief The only constructor, which open \a fname.
		 * @param[in] fname The path to the file you want to open, relative or absolute
		 * @throw std::string If this function was unable to open \a fname.
		 */
		explicit File(const std::string &fname)
		{
			this->file.open(fname.c_str());
			if (!this->file.good())
			{
				throw std::string("Error while opening ") + fname;
			}
		}
		
		//! @brief Close the internal file.
		~File(void)
		{
			this->file.close();
		}
		
		/**
		 * @brief Try to get a Container of TypeValueToRead from a single line of the internal file.
		 * @throw std::string When there is no more line to read.
		 * @return An empty container if the line doesn't contain the required value type, a full Container otherwise.
		 */
		template<typename TypeValueToRead, template<typename...> class Container=std::vector>
		Container<TypeValueToRead> readFromLine(void)
		{
			std::string line;
			Container<TypeValueToRead> container;
			if (!std::getline(this->file, line))
			{
				throw std::string("No more line to read !");
			}
			std::stringstream s(line);
			TypeValueToRead value;
			while (s >> value)
			{
				container.push_back(value);
			}
			return container;
		}
		
		/**
		 * @brief Try to get a Container of TypeValueToRead from a single line of the internal file, which contains
		 * \a n elements.
		 * @param[in] n The number of element you wish on the container.
		 * @throw std::string When there is no more line to read to seek for the \a n elements..
		 * @return A full container of \a n elements.
		 */
		template<typename TypeValueToRead, template<typename...> class Container=std::vector>
		Container<TypeValueToRead> readFromLine(int32_t n)
		{
			static std::function<bool(int32_t, int32_t)> tests[2] = {
				[](int32_t i, int32_t c) -> bool {return i != c;},
				[](int32_t i, int32_t UNUSED(c)) -> bool {return i == 0;}
			};
			Container<TypeValueToRead> container;
			while(tests[static_cast<int32_t>(n<0)](container.size(), n))
			{
				container = this->readFromLine<TypeValueToRead, Container>();
			}
			return container;
		}
		
		/**
		 * @brief Write a special line of TypeValueToWrite into the internal file.
		 * It will write a newline character at the end.
		 * @param[in] container The container of every values you wanna dump.
		 * @param[in] separator The separator between each value.
		 */
		template<typename TypeValueToWrite, template<typename...> class Container=std::vector>
		void write(const Container<TypeValueToWrite> &container, const std::string &separator)
		{
			auto it = container.begin();
			while(it != container.end())
			{
				this->file << *it;
				++it;
				if (it != container.end())
				{
					this->file << separator;
				}
			}
			this->file << std::endl;
		}
		
		//! @brief These functions are forbidden
		File(void)                                             = delete;
		File(const File<FileType> &other)                      = delete;
		File(File<FileType> &&other)                           = delete;
		File<FileType>& operator=(const File<FileType> &other) = delete;
		File<FileType>& operator=(File<FileType> &&other)      = delete;
	
};

typedef File<std::ifstream> InputFile;
typedef File<std::ofstream> OutputFile;

#endif
