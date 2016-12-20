/**
 * @file logs.hpp
 * @brief This file provides some functions to log some information.
 * 
 * It requires -std=c++11, or any further standard version, in order to compile.<br />
 * To build up documentation, just run :
 * @code
 * doxygen Doxyfile
 * @endcode
 * @author MTLCRBN
 * @version 1.2
 * @date The 12th of December 2016
 */
#ifndef LOGS_HPP_INCLUDED
#define LOGS_HPP_INCLUDED

#include <iostream>    // For std::ostream, std::endl
#include <chrono>      // For std::chrono
#include <ctime>       // For ctime(), time_t
#include <string>      // For std::string
#include <cassert>     // For assert() statement

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

#ifndef UNUSED
	#ifdef __GNUC__
		#define UNUSED(var) __attribute__((unused)) var
	#else
		// Sorry for non g++ users, you'll get an ugly warning.
		#define UNUSED(var) var
	#endif
#endif

namespace mtl
{
	namespace log
	{
		/**
		 * @brief Display every argument in \b args, one by one, with an info tag [INFO].<br />
		 * Depending of the parameter \b ENABLE_HORODATING, it could also display a date inside the tag.<br />
		 * You do not have to carry the way with template parameter.
		 * @param[in] args It's a variadic number of argument, of any type.
		 * 
		 * Example :
		 * @code
		 * int foo         = 18;
		 * std::string bar = "useless";
		 * mtl::log::info("Some information about", foo, bar, "things");
		 * // Output : [INFO   ] : Some information about 18 useless things
		 * @endcode
		 * @pre \b args must contains only variables which accept \b << operator.
		 */
		template<typename... Args>
		DECLSPEC void info(const Args&... args) noexcept;
		/**
		 * @brief Display every argument \b args, one by one, with an error tag [ERROR].<br />
		 * Depending of the parameter \b ENABLE_HORODATING, it could also display a date inside the tag.<br />
		 * You do not have to carry the way with template parameter.
		 * @param[in] args It's a variadic number of argument.
		 * 
		 * Example :
		 * @code
		 * int test = argc;
		 * if (test == 1)
		 * {
		 *  	mtl::log::error("You're suppose to provide argument while invoking", argv[0]);
		 * }
		 * // Output : [ERROR  ] :  You're suppose to provide argument while invoking ./a.out
		 * @endcode
		 * @pre \b args must contains only variables which accept \b << operator.
		 */
		template<typename... Args>
		DECLSPEC void error(const Args&... args) noexcept;
		/**
		 * @brief Display every argument in \b args, one by one, with a warning tag [WARNING].<br />
		 * Depending of the parameter \b ENABLE_HORODATING, it could also display a date inside the tag.<br />
		 * You do not have to carry the way with template parameter.
		 * @param[in] args It's a variadic number of argument.
		 * 
		 * Example :
		 * @code
		 * int test = argc;
		 * if (test == 1)
		 * {
		 *  	mtl::log::warning("You don't provide configuration file name, default configuration will be use");
		 * }
		 * // Output : [WARNING] :  You don't provide configuration file name, default configuration will be use
		 * @endcode
		 * @pre \b args must contains only variables which accept \b << operator.
		 */
		template<typename... Args>
		DECLSPEC void warning(const Args&... args) noexcept;
		
		namespace __details
		{
			class _Logger;
			
			/*
			 * This class allow to instanciate static variable member directly on the header, by playing with templates.
			 * You don't have to mess with this class.
			 * No doxygen commentary block, because I don't want to parse this class for the documentation.
			 */
			template<typename T>
			class __Static_declarer
			{
				public:
					static bool          ENABLE_HORODATING; //!< If you want the date with every line of log,   \b false      by default.
					static bool          ENABLE_LOG;        //!< If you want to switch on the logs,             \b true       by default.
					static bool          ENABLE_COLOR;      //!< If you want some colors with the tags,         \b false      by default.
					static bool          ENABLE_SPACING;    //!< If you want a space between each argument,     \b true       by default.
					static std::ostream* OUT;               //!< The std::ostream use to put logs,              \b &std::cout by default.
					static bool          ALPHA_BOOL;        //!< If you want the boolean to be display as text, \b true       by default.
				
				private:
					static const char* C_YELLOW; //!< The code for "yellow + bold"   with xterm.
					static const char* C_RED;    //!< The code for "red    + bold"   with xterm.
					static const char* C_GREEN;  //!< The code for "green  + bold"   with xterm.
					static const char* C_BLANK;  //!< The code for "normal settings" with xterm.
					
					friend class _Logger;
					template<typename... Args> friend void mtl::log::error  (const Args&... args) noexcept;
					template<typename... Args> friend void mtl::log::info   (const Args&... args) noexcept;
					template<typename... Args> friend void mtl::log::warning(const Args&... args) noexcept;
					__Static_declarer(void) = delete;
				
			};
			// Set every parameter to there default values.
			template<typename T> bool          __Static_declarer<T>::ENABLE_HORODATING = false;
			template<typename T> bool          __Static_declarer<T>::ENABLE_LOG        = true;
			template<typename T> bool          __Static_declarer<T>::ENABLE_COLOR      = false;
			template<typename T> bool          __Static_declarer<T>::ENABLE_SPACING    = true;
			template<typename T> bool          __Static_declarer<T>::ALPHA_BOOL        = true;
			template<typename T> std::ostream* __Static_declarer<T>::OUT               = &std::cout;
			template<typename T> const char*   __Static_declarer<T>::C_YELLOW          = "\033[1;33m";
			template<typename T> const char*   __Static_declarer<T>::C_RED             = "\033[1;31m";
			template<typename T> const char*   __Static_declarer<T>::C_GREEN           = "\033[1;32m";
			template<typename T> const char*   __Static_declarer<T>::C_BLANK           = "\033[0m";
		}
		
		/**
		 * @brief Embeds parameters as public static members, so you could easily change (or mess with) settings.
		 * @warning It isn't recommended to enable color when you want to dump logs to a file
		 * because it may causes issues while parsing these files later.
		 * @warning \b Colors are provide by using \b xterm standards, it may not work for other shells.
		 * @warning \b OUT may be an usable and valid \b std::ostream, there is no warranty if you mess up with that.
		 * 
		 * Some examples :
		 * @code
		 * mtl::log::Options::ENABLE_COLOR      = true; // Or false.
		 * mtl::log::Options::ENABLE_LOG        = true; // Or false.
		 * mtl::log::Options::ENABLE_HORODATING = true; // Or false.
		 * mtl::log::Options::ENABLE_COLOR      = true; // Or false.
		 * mtl::log::Options::ALPHA_BOOL        = true; // Or false.
		 * 
		 * std::ofstream                         file(...); // Open the file before, of course.
		 * std::ostringstream                    os;
		 * struct MyOstream : std::ostream {...} mos;
		 * mtl::log::Options::OUT = &file;
		 * mtl::log::Options::OUT = &os;
		 * mtl::log::Options::OUT = &mos;
		 * mtl::log::Options::OUT = &std::cerr;
		 * // Keep in mind, the pointer given to OUT must remain valid, otherwise it could produce impredictible behavior.
		 * // Keep also in mind than std::ostream is protected by default, that mean than "std::ostream os;" won't work !
		 * @endcode
		 */
		class DECLSPEC Options final : public __details::__Static_declarer<Options>
		{
			private:
				Options(void) = delete;
			
		};
		
		namespace __details
		{
			/*
			 * Just a single structure to define  a hold_on tag.
			 * As usual, don't even try to use it raw, this is pointless.
			 */
			struct _HoldOn final
			{
				_HoldOn(void)                            = default;
				_HoldOn(_HoldOn&& other)                 = default;
				_HoldOn(const _HoldOn& other)            = default;
				_HoldOn& operator=(const _HoldOn& other) = delete;
				_HoldOn& operator=(_HoldOn&& other)      = delete;
			};
			//! @cond HIDE_THIS_DOXYGEN
			// All different states for a log channel such as info, error, warning.
			typedef enum
			{
				SKIP,    //!< The state to skip a call.
				NOTHING, //!< The usual state, nothing special.
				HOLD     //!< The state to hold on for the next call.
			} _flags_e;
			
			// To know which channel is used.
			typedef enum
			{
				ERROR,   //!< Error   channel is used.
				WARNING, //!< Warning channel is used.
				INFO     //!< Info    channel is used.
			} _current_e;
			//! @endcond
			
			/*
			 * Embeds the channels states.
			 * As usual, don't even try to use it raw, this is pointless.
			 */
			template<typename Foo>
			struct __States_declarer
			{
				protected:
					static _flags_e   info_f;    //!< The state of the info    channel.
					static _flags_e   warning_f; //!< The state of the warning channel.
					static _flags_e   error_f;   //!< The state of the error   channel.
					static _current_e curr_c;    //!< Which channel is used.
					static bool       isHolding; //!< To indicate if the current input beg for holding on.
					__States_declarer(void) = delete;
			};
			
			// States initialisation.
			template<typename Foo> _flags_e   __States_declarer<Foo>::info_f    = NOTHING;
			template<typename Foo> _flags_e   __States_declarer<Foo>::error_f   = NOTHING;
			template<typename Foo> _flags_e   __States_declarer<Foo>::warning_f = NOTHING;
			template<typename Foo> _current_e __States_declarer<Foo>::curr_c    = INFO;
			template<typename Foo> bool       __States_declarer<Foo>::isHolding = false;
			
			/*
			 * This class protects the common part of error(), warning() and info().
			 * You cannot mess with this class, sorry.
			 * No doxygen commentary block, because I don't want to parse this class for the documentation.
			 */
			class _Logger final : public __States_declarer<_Logger>
			{
				private:
					_Logger(void) = delete;
					template<typename... Args> friend void mtl::log::error  (const Args&... args) noexcept;
					template<typename... Args> friend void mtl::log::info   (const Args&... args) noexcept;
					template<typename... Args> friend void mtl::log::warning(const Args&... args) noexcept;
					
					/**
					 * @brief Terminal case, this function is call when there is no more arguments.<br />
					 * So, at the end, it displays a newline.
					 */
					static void _print_(void) noexcept
					{
						if (_Logger::isHolding)
						{
							_Logger::isHolding = false;
							*mtl::log::Options::OUT << std::flush;
						}
						else
						{
							*mtl::log::Options::OUT << std::endl;
						}
					}
					/**
					 * @brief Changes the state of \b f if the current value is HOLD.
					 * @param[in,out] f The current flag to check.
					 */
					static void changeState(_flags_e& f) noexcept
					{
						if (f == HOLD)
						{
							*mtl::log::Options::OUT << std::endl;
							f = SKIP;
						}
					}
					/**
					 * @brief Change the curr_c parameter and manage flags.
					 * @param c The new channel you're using.
					 */
					static void setCurrent(const _current_e c) noexcept
					{
						if (c != _Logger::curr_c)
						{
							_Logger::changeState(info_f);
							_Logger::changeState(error_f);
							_Logger::changeState(warning_f);
							_Logger::curr_c = c;
						}
					}
					/**
					 * @brief General case, it will display the head of the \b args list \b a, and make a recursive call
					 * with the rest of the list.
					 * @param[in] a    The current argument to display, of any type.
					 * @param[in] args The rest of the argument list.
					 */
					template<typename Actual, typename... Args>
					static void _print_(const Actual& a, const Args&... args) noexcept
					{
						if (mtl::log::Options::ENABLE_SPACING)
						{
							*mtl::log::Options::OUT << ' ';
						}
						*mtl::log::Options::OUT << a;
						_Logger::_print_(args...);
					}
					/**
					 * @brief Specific case, encountering the hold_on tag end the recursion prematuraly.
					 * @param[in] tag  The hold_on tag, just here to catch this case (unused actually).
					 * @param[in] args The rest of the argument list.
					 */
					template<typename... Args>
					static void _print_(UNUSED(const _HoldOn& tag), const Args&... args) noexcept
					{
						#ifndef __GNUC__
							// Nah, I'm such a nice guy, you won't get any warning.
							(void)tag;
						#endif
						_Logger::isHolding = true;
						switch(_Logger::curr_c)
						{
							case WARNING:
								_Logger::warning_f = HOLD;
								break;
							case ERROR:
								_Logger::error_f = HOLD;
								break;
							default:
								_Logger::info_f = HOLD;
						}
						_Logger::_print_(args...);
					}
					//! @brief Ensures the validity of \b OUT.
					static void assertValidity(void) noexcept
					{
						assert(mtl::log::Options::OUT != nullptr && "OUT must not be nullptr !");
						assert(mtl::log::Options::OUT->good()    && "OUT must be a \"good()\" std::ostream* !");
					}
					/**
					 * @brief Start to read the argument list \b args, displays the tag \b tag with the specified color \b color.
					 * The color appears only if \b ENABLE_COLOR is set to true.
					 * @param[in] tag   The text to display inside the tag at the beginning of the line.
					 * @param[in] color The color code to use when the tag is display.
					 * @param[in] args  The argument list.
					 * @pre \b OUT must not be nullptr.
					 * @pre \b OUT must be a valid std::ostream (OUT->good() must return true).
					 */
					template<typename... Args>
					static void _start_print(const char *const tag, const char *const color, const Args&... args) noexcept
					{
						_Logger::assertValidity();
						if (mtl::log::Options::ENABLE_LOG)
						{
							if (mtl::log::Options::ALPHA_BOOL)
							{
								*mtl::log::Options::OUT << std::boolalpha;
							}
							else
							{
								*mtl::log::Options::OUT << std::noboolalpha;
							}
							*mtl::log::Options::OUT << '[';
							if (mtl::log::Options::ENABLE_COLOR)
							{
								*mtl::log::Options::OUT << color;
							}
							*mtl::log::Options::OUT << tag;
							if (mtl::log::Options::ENABLE_COLOR)
							{
								*mtl::log::Options::OUT << ::mtl::log::Options::C_BLANK;
							}
							if (mtl::log::Options::ENABLE_HORODATING)
							{
								time_t tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
								std::string date(ctime(&tt));
								date.pop_back();
								*mtl::log::Options::OUT << ", " << date;
							}
							*mtl::log::Options::OUT << "] :";
							if (!mtl::log::Options::ENABLE_SPACING)
							{
								*mtl::log::Options::OUT << ' ';
							}
							_Logger::_print_(args...);
						}
					}
					/**
					 * @brief Encapsulates a call to generalize this part.
					 * @param[in]     v     The current value to affect at \b _Logger.
					 * @param[in,out] f     The flag to manage.
					 * @param[in]     tag   The tag you wanna display.
					 * @param[in]     color The color of the tag.
					 * @param[in]     args  Every argument to display.
					 */
					template<typename... Args>
					static void call(_current_e v, _flags_e& f, const char *const tag, const char *const color, const Args&... args)
					{
						if (f == NOTHING)
						{
							_Logger::setCurrent(v);
							_Logger::_start_print(tag, color, args...);
						}
						else if(f == HOLD)
						{
							f = NOTHING;
							_Logger::_print_(args...);
						}
						else
						{
							f = NOTHING;
						}
					}
			};
			
		}
		/**
		 * @brief Return a hold_on tag.
		 * It will prevent the current line that it's not the end for this log line, and wait for something
		 * else before printing the newline.
		 * @code
		 * mtl::log::info("Loading file ...", mtl::log::hold_on()); // Display only [INFO] : Loading file ...
		 * // Do stuff while loading the file.
		 * mtl::log::info("done"); // add done at the end of the previous output.
		 * // Display [INFO] : Loading file ... done
		 * @endcode
		 * Keep in mind, if you call another log method (like, in our case, error()), it will cancel the
		 * "done" part in that case (the next call to be more precise) !
		 * @return The hold_on tag.
		 */
		DECLSPEC inline __details::_HoldOn hold_on(void) noexcept
		{
			return __details::_HoldOn();
		}
		template<typename... Args>
		DECLSPEC void error(const Args&... args) noexcept
		{
			__details::_Logger::call(__details::ERROR, __details::_Logger::error_f, "ERROR  ", Options::C_RED, args...);
		}
		template<typename... Args>
		DECLSPEC void info(const Args&... args) noexcept
		{
			__details::_Logger::call(__details::INFO, __details::_Logger::info_f, "INFO   ", Options::C_GREEN, args...);
		}
		template<typename... Args>
		DECLSPEC void warning(const Args&... args) noexcept
		{
			__details::_Logger::call(__details::WARNING, __details::_Logger::warning_f, "WARNING", Options::C_YELLOW, args...);
		}
	}
}

#endif

/**
 * @mainpage Logs 1.2
 * 
 * @section id_intro Synopsis
 * This Header-only library provides logging configurable functions that allows you to customize output format.<br/>
 * Because this is only a header, there is no need to compile anything to use this library.<br />
 * Examples are far better than words, let's get it started !
 * @section id_examples Usage
 * @subsection id_cc   How it compiles ?
 * In order to use this library, you only need a C++ standard of \b c++11 or any further !
 * @code
 * -std=c++11
 * @endcode
 * And that's all you need about compilation.<br/>
 * 
 * @subsection id_how   How it works ?
 * To begin with, you'll need to include the library from your local project source files:
 * @code
 * #include "logs.hpp"
 * @endcode
 * or even better, move the header to your local system library directory and then include it :
 * @code
 * #include <logs.hpp>
 * @endcode
 * <br />
 * Then, because it belongs to my own namespace, a <b>namespace alias</b> may be handy :
 * @code
 * namespace log = mtl::log;
 * @endcode
 * You're now able to use my library :D<br /><br />
 * Fine, let's log something cool in age.<br />
 * In order to do so, you just need to call the \b log::info() statement, and precise any
 * argument you want to log, each argument separated with a coma.
 * @code
 * std::string myName = "MTLCRBN";
 * log::info("Hello world from", myName);
 * @endcode
 * Which generates this output :
 * @code
 * [INFO] : Hello world from MTLCRBN
 * @endcode
 * There are 2 others statements, for different purpose. At least, you have to choose between :
 *     - info()
 *     - warning()
 *     - error()
 *
 * @code
 * std::string myName = "MTLCRBN";
 * log::info   ("Hello world from", myName);
 * log::error  ("Hello world from", myName, "as an error message");
 * int nbError = 5;
 * log::warning("Hello world from", myName, "with", nbError, "warnings");
 * @endcode
 * Output :
 * @code
 * [INFO   ] :  Hello world from MTLCRBN
 * [ERROR  ] :  Hello world from MTLCRBN as an error message
 * [WARNING] :  Hello world from MTLCRBN with 5 warnings
 * @endcode
 * <br /><br />
 * Ok, but what about the parameters I talked about earlier?<br />
 * Actually, available parameters are :
 * <table>
 * 	<tr>
 * 		<td>Parameter name</td>
 * 		<td>Legal values</td>
 * 		<td>Default value</td>
 * 		<td>Description</td>
 * 	</tr>
 * 	<tr>
 * 		<td><b>ENABLE_HORODATING</b></td>
 * 		<td><b>true</b> or <b>false</b></td>
 * 		<td>false</td>
 * 		<td>
 * 			Enable/Disable date in the braces.
 * 			@code
 * 			[WARNING, Fri Dec  2 22:00:43 2016]
 * 			@endcode
 * 			The date format is : <b>dayOfWeek month dayNumber hours:minutes:seconds year</b>
 * 		</td>
 * 	</tr>
 * 	<tr>
 * 		<td><b>ENABLE_LOG</b></td>
 * 		<td><b>true</b> or <b>false</b></td>
 * 		<td>true</td>
 * 		<td>Enable/Disable logs for further calls of info(), warning(), error()</td>
 * 	</tr>
 * 	<tr>
 * 		<td><b>ENABLE_SPACING</b></td>
 * 		<td><b>true</b> or <b>false</b></td>
 * 		<td>true</td>
 * 		<td>if set to \b true, it will add an extra space between each argument</td>
 * 	</tr>
 * 	<tr>
 * 		<td><b>ENABLE_COLOR</b></td>
 * 		<td><b>true</b> or <b>false</b></td>
 * 		<td>false</td>
 * 		<td>
 * 			activates \b xterm color codes for in between braces text.<br />
 * 			It may not be use for log dumping, in order to prevent further troubles when parsing files.<br />
 * 			It may not work with other terminals, carry on.
 * 		</td>
 * 	</tr>
 *  <tr>
 * 		<td><b>ALPHA_BOOL</b></td>
 * 		<td><b>true</b> or <b>false</b></td>
 * 		<td>true</td>
 * 		<td>This parameter allows you to display \b "true" instead of \b "1" when you display a \b bool with log functions.</td>
 * 	</tr>
 * 	<tr>
 * 		<td><b>OUT</b></td>
 * 		<td>Any valid std::ostream address (<b>std::ostream*</b>)</td>
 * 		<td>&std::cout</td>
 * 		<td>Set which std::ostream you'll use to log.</td>
 * 	</tr>
 * </table>
 * They are grouped and accessible in the \b mtl::logs::Options class. Accessing this class allows you to set their values and then customize your output.
 * Example :
 * @code
 * log::Options::ENABLE_LOG = true;  // Enable  logs for further calls of info(), warning(), error().
 * log::Options::ENABLE_LOG = false; // Disable logs for further calls of info(), warning(), error().
 * // log::Options::ENABLE_... = value;
 * @endcode
 * \b OUT parameter is a bit more complicated. It defines the \b std::ostream where the log should be dumped<br/>
 * It can be changed by giving the address of any \b valid std::ostream, of any types :
 * @code
 * log::Options::OUT = &std::cerr; // To log on stderr
 * std::ostringstream os;
 * log::Options::OUT = &os; // Carry on with destructors, scoped variables, etc.
 * std::ofstream myFile("mylogfile.log");
 * log::Options::OUT = &myFile; // To log onto a file.
 * @endcode
 * Changing this parameter is up to you, any misbehavior is on your own responsability.
 * @subsection id_hold Hold on a second !
 * But MTLCRBN, that's fine for me, but I'm use to display a message to indicate I'm starting some huge work, and then indicate
 * a "OK" message at the end if everything goes well, like this :
 * @code
 * std::cout << "Starting parsing this huge gigantic file ... " << std::flush;
 * // Atrocious stuff, it takes forever.
 * std::cout << "OK" << std::endl;
 * @endcode
 * For an output which looks like this :
 * @code
 * Starting parsing this huge gigantic file ... OK
 * @endcode 
 * But, with your library, I'm oblige to print 2 lines instead of one, that's crap !
 * 
 * 
 * Let's correct this, it WAS crap.<br />
 * This is possible to accomplish this task with my library since the 1.2 version.<br />
 * You just need to tell at my library : "wait a minut, something else incoming later". You can do that with the 
 * hold_on() statement.
 * @code
 * mtl::log::info("This is the answer to the big question =>", mtl::log::hold_on());
 * // Some years later
 * mtl::log::info("42");
 * @endcode
 * It will produce what you expect :
 * @code
 * [INFO] : This is the answer to the big question =>  42
 * @endcode
 * 
 * It's important to know somethings about this method :
 *    - It will wait for the next call to the \b same log method.
 *      If you call info() for example, and then error(), the next info("with_answer") will be cancel.
 *    - Ensure that there isn't any other info() during the treatment !
 * 
 * By the way, you could put the hold_on() statement where you want on the arguments.
 * @code
 * mtl::log::info(mtl::log::hold_on(), "whatever"); // It will work
 * @endcode
 * 
 * @subsection id_sum   Summary
 * There is a full code to demonstrate the usage :
 * @code
 * #include "logs.hpp"
 * #include <string>
 * #include <iostream>
 * 
 * namespace log = mtl::log;
 * 
 * int main(int argc, char** argv)
 * {
 *  	log::Options::ENABLE_COLOR      = true;       // We do have an xterm, so let's turn on colors.
 *  	log::Options::ENABLE_SPACING    = false;      // Let us deal with spacing between each argument.
 *  	log::Options::ENABLE_HORODATING = true;       // We do want dates to appear.
 *  	log::Options::OUT               = &std::cerr; // let's log on std::cerr, because why not.
 *  	
 *  	log::info("Hello world !");
 *  	
 *  	std::string swear = "Because you must do it !";
 *  	if (argc == 1)
 *  		log::error("You forgot to provide arguments, ", swear);
 *  	
 *  	int maxLogs = 15000;
 *  	log::Options::ENABLE_SPACING = true;
 *  	log::warning("Heavy logs incoming.", maxLogs, "messages on sight !");
 *  	
 *  	log::Options::ENABLE_LOG = false; // Fiou, we'll avoid those tons of useless logs.
 *  	for(int i=0;i<maxLogs;++i)
 *  		log::info("This is line number", i);
 *      log::Options::ENABLE_LOG = true;
 *      log::info("Let's count to 5 -->", log::hold_on());
 *      for(int i=1;i<6;i++)
 *          log::info(log::hold_on(), i);
 *      log::info("--> Done :D");
 *  	return 0;
 * }
 * @endcode
 * And the mighty output is :
 * 
 * [<b style="color:green">INFO</b>   , Fri Dec  2 22:28:41 2016] : Hello world !<br />
 * [<b style="color:red">ERROR</b>  , Fri Dec  2 22:28:41 2016] : You forgot to provide arguments, Because you must do it !<br />
 * [<b style="color:yellow">WARNING</b>, Fri Dec  2 22:28:41 2016] :  Heavy logs incoming. 15000 messages on sight !<br />
 * [<b style="color:green">INFO</b>   , Fri Dec  2 22:28:41 2016] : Let's count to 5 --> 1 2 3 4 5 --> Done :D
 *
 * 
 * @section id_doc      Documentation
 * The documentation is build with Doxygen.
 * @section id_license  License
 * This "software" is provided without any express or implied warranty.<br />
 * In no event will the author be held liable for any damages arising from the use of this "software".<br />
 * Anyway, feel free to enhance or modify it as you please, this is not my business anymore.
 * @section id_author Author & contributor(s)
 * <b>MTLCRBN</b><br />
 * <b>Zelnes</b>, for the english check of this documentation (thanks buddy), and for some ideas.
 */
