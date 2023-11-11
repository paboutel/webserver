/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/21 10:25:37 by hrecolet          #+#    #+#             */
/*   Updated: 2022/11/12 12:24:59 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "IOpoll.hpp"
# include <map>
# include <string>
# include <vector>

class Request {
	public:
		typedef std::map< std::string, std::vector<std::string> > request_type;
		
	private:
		static const std::map<std::string, void(Request::*)(std::string &)> _splitTable;
		request_type				_header;
		std::string					_method;
		std::string					_target;
		std::string					_version;
		std::vector<char>			_body;
		std::vector<std::string>	_argvVar;
		std::string					_envVar;
		std::string					_uploadFileName;

		std::vector<char>			_rawData;
		static size_t				_maxHeaderSize;
		int							_fd;
		
		void	_basicSplit(std::string &line);
		void	_secSplit(std::string &line);
		void	_userSplit(std::string &line);
		void	_acceptSplit(std::string &line);
		void	_parseFirstLine(const std::string &line);
		void	_printValue(void);
		void	_parseHeader(std::string &body);
		void	_parseBody(std::vector<char> body);
		bool	_chunkedRequest(std::vector<std::string> &vecBody);
		bool	_uploadRequest(std::string &body);
		std::vector<char>::iterator	_vectorCharSearch(std::vector<char>::iterator first, std::vector<char>::iterator last, std::string toFind);
		void	_unchunkedRequest(std::vector<char> &body);
		void	_parseFileName(std::vector<char> &body);
		std::vector<char>::iterator	_searchLastLine(std::vector<char> &body);
		void	_checkHeader(void);
		bool	_isHeaderComplete(void);
		void	_fillHeader(void);
		bool	_isBodyComplete(void);
		void	_fillBody(void);



	public:
		Request(int fd = -1);
		~Request();

		request_type				getData(void) const;
		std::vector<std::string>	&getVar(void);
		std::string					getMethod(void) const;
		std::string					getTarget(void) const;
		std::string					getVersion(void) const;
		std::vector<char>			getBody(void) const;
		std::string					getEnvVar(void);
		std::string					getUploadFileName(void);

		bool						rec(void);


		class RequestError: public std::exception {
			private:
				std::string	_error;
			public:
				RequestError(const std::string &error)
				: _error(error) {}

				virtual ~RequestError(void) throw() {}

				virtual const char *what() const throw()
				{ return (_error.c_str()); }
		};
		
		class InvalidHeader: public std::exception {
			private:
				std::string	_error;
			public:
				InvalidHeader(const std::string &error)
				: _error(error) {}

				virtual ~InvalidHeader(void) throw() {}

				virtual const char *what() const throw()
				{ return (_error.c_str()); }
		};
};
