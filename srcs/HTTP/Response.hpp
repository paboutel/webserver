/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/21 11:47:56 by hrecolet          #+#    #+#             */
/*   Updated: 2022/11/08 06:32:07 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "Request.hpp"
# include "ConfigParser.hpp"
# include <fstream>

#define NB_MIME 66

class Response {
	private:
		ConfigParser::Location		_env;
		Request						_req;
		std::vector<std::string>	_var;
		std::string					_clientIp;
		char						**_sysEnv;
		std::vector<char>			_data;
		std::string					_code;
		std::string					_status;
		std::string					_header;
		std::string					_types;
		std::string					_filename;
		std::vector<std::string>	_cookies;

		std::map<std::string, std::string>			_mimeTypes;
		void										init_mimeTypes(void);
		std::map<std::string, void(Response::*)()>	_methodsFunction;
		void										init_methodsFunction(void);

		std::string			_getDate(void);
		std::vector<char>	_getDefaultErrorPage(void);
		void				_setType(std::string url);
		bool				_isBinaryFile(std::string filePath);
		void				_readFile(std::string &filename);
		bool				_isFileAccessible(std::string filename);
		void				_execGet(void);
		void				_execDel(void);
		bool				_isCgiFile(std::string root);
		int					_execCgi(std::string root);
		void				_setError(std::string code);
		bool				_checkFile(std::string filename);
		void				_readPipe(int pipeToRead);
		std::string			_findCgiPath(std::string root);
		void				_uploadFile(void);
		void				_writeFile(void);


	public:
		const static std::string	_css;
		Response(ConfigParser::Location env, Request req, std::string clientIp, char **sysEnv);
		Response() {};

		void	constructData(void);
		void	execute(void);
		bool	sendData(int clientFd);
		void	setData(std::vector<char> data);

		class ResponseError: public std::exception {
			private:
				std::string	_error;
			public:
				ResponseError(const std::string &error)
				: _error(error) {}

				virtual ~ResponseError(void) throw() {}

				virtual const char *what() const throw()
				{ return (_error.c_str()); }
		};
};
