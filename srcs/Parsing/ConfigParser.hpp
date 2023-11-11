/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 11:36:05 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/12 21:13:12 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <vector>
# include <map>
# include <set>
# include <string>
# include <iostream>

class ConfigParser {
	private:
		typedef std::string::iterator								lineIt_type;
		typedef std::vector<std::string>::iterator					fileIt_type;
		typedef std::pair<lineIt_type, lineIt_type>					lineRange_type;
		typedef std::pair<fileIt_type, fileIt_type>					fileRange_type;
		typedef std::pair< std::string, std::vector<std::string> >	keyValues_type;

		
	public:

		struct Location
		{
			typedef std::map< std::string, std::map< std::string, std::vector<std::string> > >	nonUniqKey_type;
			typedef std::map< std::string, std::vector<std::string> >							uniqKey_type;
			
			nonUniqKey_type	nonUniqKey;
			uniqKey_type	uniqKey;

			static void			_insertUniqKey(const uniqKey_type &a, uniqKey_type &b)
			{
				for (uniqKey_type::const_iterator it = a.begin(); it != a.end(); it++)
					b.insert(*it);
			}

			void			insert(const Location &otherInst)
			{
				_insertUniqKey(otherInst.uniqKey, uniqKey);
				for (nonUniqKey_type::const_iterator it = otherInst.nonUniqKey.begin(); it != otherInst.nonUniqKey.end(); it++)
					_insertUniqKey(it->second, nonUniqKey[it->first]);
			}
		};

		struct Server
		{
			typedef std::map< std::string, std::set<std::string> >	listen_type;
			typedef std::set<std::string>							server_name_type;
			typedef std::map<std::string, Location>					location_type;

			listen_type			listen; // map d'{Ip, set<port>}
			server_name_type	server_name; // set de server_name
			location_type		location;
		};

	private:
		struct Conf
		{
			enum KeyType {KT_NONE, KT_SERVER, KT_UNIQ, KT_NON_UNIQ};

			struct raw
			{
				KeyType						kt;
				void						(*func)(keyValues_type &, size_t &startLastLine, std::string &line);
				size_t						maxParams;
				std::set<std::string>		validParams;

				raw(void) {};

				raw(const KeyType &kt, void (*func)(keyValues_type &, size_t &startLastLine, std::string &line), size_t maxParams, std::string validParamsTab[], size_t validParamsSize)
				: kt(kt), func(func), maxParams(maxParams), validParams(validParamsTab, validParamsTab + validParamsSize) {};

				raw(const KeyType &kt, void (*func)(keyValues_type &, size_t &startLastLine, std::string &line), size_t maxParams)
				: kt(kt), func(func), maxParams(maxParams) {};
			};

			typedef std::map<std::string, raw>	data_type;

			static data_type	_data;
			static void			init_data(void);

			static Location		_defaultValues;
			static void			init_defaultValues(void);

			const static std::string	_whitespacesSet;
			const static std::string	_lineBreakSet;
			const static std::string	_commentSet;
			const static std::string	_scopeSet;

			static KeyType	getKeyType(const std::string &key)
			{ return ((_data.count(key)) ? _data[key].kt : KT_NONE); }
		};

	public:

		typedef std::vector<Server> 						data_type;
		typedef std::map< std::string, std::vector<std::string> >	map_type;
		
	private:
		data_type	_data;
	
		//parseFunction
		void			_skipCharset(lineRange_type &lineRange, const std::string &charset);
		std::string		_getWordSkipSpace(lineRange_type &strIt);
		std::string		_getWord(lineRange_type &strIt);
		void			_goToNextWordInFile(lineRange_type &strIt, fileRange_type &fileIt);
		bool			_isServer(keyValues_type keyValues, lineRange_type &lineRange, fileRange_type &fileRange);
		bool			_isLocation(keyValues_type pair, lineRange_type &strIt, fileRange_type &fileIt, size_t startLastLine);
		Server			_createNewServer(lineRange_type &strIt, fileRange_type &fileIt);
		Location		_createNewLocation(lineRange_type &lineRange, fileRange_type &fileRange);
		keyValues_type	_getKeyValues(lineRange_type &strIt);
		void			_insertKeyValuesInLocation(Location &location, keyValues_type &keyValues, size_t &startLastLine, std::string &line);
		void			_insertKeyValuesInServer(Server &res, keyValues_type &keyValues, size_t &startLastLine, std::string &line);

		bool			checkDuplicatedParams(std::vector<std::string> params, size_t &startLastLine, std::string &line);
		bool			checkValidParams(std::vector<std::string> params, std::set<std::string> validParams, size_t &startLastLine, std::string &line);
		void			checkKeyValues(keyValues_type &keyValues, const Conf::raw &keyConf, size_t startLastLine, std::string &line);

		//Check Functions
		static void	_konamiCode(keyValues_type &keyValues, size_t &startLastLine, std::string &line);
		static void	_checkBodySize(keyValues_type &keyValues, size_t &startLastLine, std::string &line);
		static void	_checkCgi(keyValues_type &keyValues, size_t &startLastLine, std::string &line);
		static void	_checkRoot(keyValues_type &keyValues, size_t &startLastLine, std::string &line);

		static void checkIp(std::vector<std::string> ip, size_t &startLastLine, std::string &line);
		static void checkPort(std::string str, size_t &startLastLine, std::string &line);
		static void	formatListen(keyValues_type &keyValues, size_t &startLastLine, std::string &line);

		static void	_colorSkipFirstWordInRange(size_t &first, const std::string &word, std::string &line, const std::string &color);
		
	public:
		ConfigParser(char *params);

		//accessor
		data_type	getData(void);
		
		class ParsingError: public std::exception {
			private:
				std::string	_error;
				std::string	_word;
			public:
				ParsingError(const std::string &error, const std::string &word = std::string())
				: _error(error), _word(word) {}

				virtual ~ParsingError(void) throw() {}

				virtual const char *what() const throw()
				{ return (_error.c_str()); }

				std::string	word(void)
				{ return (_word); }
		};
};
