/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 15:23:33 by hrecolet          #+#    #+#             */
/*   Updated: 2022/11/13 14:43:34 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "CgiHandler.hpp"
#include "utils.hpp"
#include <sys/stat.h>
#include <sys/socket.h>
#include <algorithm>
#include <unistd.h>
#include <iostream>

#define SIZEOF(arr) sizeof(arr) / sizeof(*arr)
#define C_RED "\033[1;31m"
#define C_GREEN "\033[1;32m"
#define C_RESET "\033[0m"

void	Response::init_mimeTypes(void)
{
	if (!_mimeTypes.empty())
		return ;
	std::pair<std::string, std::string>	mimeTypes[] =
	{
		std::make_pair(".aac", "audio/aac"),
		std::make_pair(".abw", "application/x-abiword"),
		std::make_pair(".arc", "application/octet-stream"),
		std::make_pair(".avo", "video/x-msvideo"),
		std::make_pair(".bin", "application/octet-stream"),
		std::make_pair(".bmp", "image/bmp"),
		std::make_pair(".bz", "application/x-bzip"),
		std::make_pair(".bz2", "application/x-bzip2"),
		std::make_pair(".csh", "application/x-csh"),
		std::make_pair(".css", "text/css"),
		std::make_pair(".csv", "text/csv"),
		std::make_pair(".doc", "application/msword"),
		std::make_pair(".docx", "application/vnd.openxmlformats-officedocument.wordprocessingml.document"),
		std::make_pair(".eot", "application/vnd.ms-fontobject"),
		std::make_pair(".epub", "application/epub+zip"),
		std::make_pair(".gif", "image/gif"),
		std::make_pair(".htm", "text/html"),
		std::make_pair(".html", "text/html"),
		std::make_pair(".ico", "image/x-icon"),
		std::make_pair(".ics", "text/calendar"),
		std::make_pair(".jar", "application/java-archive"),
		std::make_pair(".jpeg", "image/jpeg"),
		std::make_pair(".jpg", "image/jpeg"),
		std::make_pair(".js", "application/javascript"),
		std::make_pair(".json", "application/json"),
		std::make_pair(".mid", "audio/midi"),
		std::make_pair(".midi", "audio/midi"),
		std::make_pair(".mpeg", "video/mpeg"),
		std::make_pair(".mpkg", "application/vnd.apple.installer+xml"),
		std::make_pair(".odp", "application/vnd.oasis.opendocument.presentationa"),
		std::make_pair(".ods", "application/vnd.oasis.opendocument.spreadsheet"),
		std::make_pair(".odt", "application/vnd.oasis.opendocument.text"),
		std::make_pair(".oga", "audio/ogg"),
		std::make_pair(".ogv", "video/ogg"),
		std::make_pair(".ogx", "application/ogg"),
		std::make_pair(".otf", "font/otf"),
		std::make_pair(".png", "image/png"),
		std::make_pair(".pdf", "application/pdf"),
		std::make_pair(".ppt", "application/vnd.ms-powerpoint"),
		std::make_pair(".pptx", "application/vnd.openxmlformats-officedocument.presentationml.presentation"),
		std::make_pair(".rar", "application/x-rar-compressed"),
		std::make_pair(".rtf", "application/rtf"),
		std::make_pair(".sh", "application/x-sh"),
		std::make_pair(".svg", "image/svg+xml"),
		std::make_pair(".swf", "application/x-shockwave-flash"),
		std::make_pair(".tar", "application/x-tar"),
		std::make_pair(".tif", "image/tiff"),
		std::make_pair(".tiff", "image/tiff"),
		std::make_pair(".ts", "application/typescript"),
		std::make_pair(".ttf", "font/ttf"),
		std::make_pair(".vsd", "application/vnd.visio"),
		std::make_pair(".wav", "audio/x-wav"),
		std::make_pair(".weba", "audio/webm"),
		std::make_pair(".webm", "video/webm"),
		std::make_pair(".webp", "image/webp"),
		std::make_pair(".woff", "font/woff"),
		std::make_pair(".woff2", "font/woff2"),
		std::make_pair(".xhtml", "application/xhtml+xml"),
		std::make_pair(".xls", "application/vnd.ms-excel"),
		std::make_pair(".xlsx", "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"),
		std::make_pair(".xml", "application/xml"),
		std::make_pair(".xul", "application/vnd.mozilla.xul+xml"),
		std::make_pair(".zip", "application/zip"),
		std::make_pair(".3gp", "video/3gpp"),
		std::make_pair(".3g2", "video/3gpp2"),
		std::make_pair(".7z", "application/x-7z-compressed")
	};
	_mimeTypes.insert(mimeTypes, mimeTypes + SIZEOF(mimeTypes));
}

void	Response::init_methodsFunction(void)
{
	if (!_methodsFunction.empty())
		return ;

	std::pair<std::string, void(Response::*)()>	methodsFunction[] =
	{
		std::make_pair("GET", &Response::_execGet),
		std::make_pair("POST", &Response::_execGet),
		std::make_pair("DELETE", &Response::_execDel),
	};
	_methodsFunction.insert(methodsFunction, methodsFunction + SIZEOF(methodsFunction));
}

/* -------------------------------------------------------------------------- */
/*                                 constructor                                */
/* -------------------------------------------------------------------------- */

Response::Response(ConfigParser::Location env, Request req, std::string clientIp, char **sysEnv)
: _env(env), _req(req), _var(req.getVar()), _clientIp(clientIp), _sysEnv(sysEnv), _code("0")
{
	init_mimeTypes();
	init_methodsFunction();
}

/* -------------------------------------------------------------------------- */
/*                                   methods                                  */
/* -------------------------------------------------------------------------- */

const std::string Response::_css("\
			<style>\
				body{\
					font-family: 'Calistoga', cursive;\
					background-color:#282a39;\
					color:#bd93f9;\
				}\
				a {\
					text-decoration: none;\
				}\
			</style>"\
		);

std::vector<char>	Response::_getDefaultErrorPage(void)
{
	std::string	page("\
		<!DOCTYPE html>\
		<html>\
		<title>Online HTML Editor</title>\
		<head>\
		<link rel=\"preconnect\" href=\"https://fonts.googleapis.com\">\
		<link rel=\"preconnect\" href=\"https://fonts.gstatic.com\" crossorigin>\
		<link href=\"https://fonts.googleapis.com/css2?family=Calistoga&display=swap\" rel=\"stylesheet\">\
		" + _css + "\
		</head>\
		<body>\
			<center>\
				<h2>" + _code + "</h2>\
				<p>" + _status + "</p>\
				<IMG SRC=\"https://media.tenor.com/Fmoc_-YxLWIAAAAC/shoes-cat.gif\">\
			</center>\
		</body>\
		</html>"\
	);
	return (std::vector<char> (page.begin(), page.end()));
}

std::string	Response::_getDate(void) {
	time_t rawtime;
	struct tm * timeinfo;
	char buffer [80];
	
	time (&rawtime);
	if (rawtime == (time_t)-1)
		throw ResponseError("Response error: time() failed");
	timeinfo = localtime (&rawtime);
	if (strftime (buffer, 80, "%a, %e %b %G %T GMT",timeinfo) == 0)
		throw ResponseError("Response error: strftime() failed");
	return (std::string(buffer));
}

void	Response::_setError(std::string code)
{
	_code = code;
	_status = _env.nonUniqKey["return"][_code][0];
	if (_env.nonUniqKey["error_page"].count(code) && _checkFile(_env.nonUniqKey["error_page"][_code][0]) == false)
		_readFile(_env.nonUniqKey["error_page"][_code][0]);
	else
		_data = _getDefaultErrorPage();
	_types = "text/html";
}

bool	Response::_checkFile(std::string filename)
{
	struct stat buf;
	
	stat(filename.c_str(), &buf);
	if (access(filename.c_str(), F_OK) != 0)
	{
		if (_code == "0")
			_setError("404");
		return (true);
	}
	if (access(filename.c_str(), R_OK) != 0)
	{
		if (_code == "0")
			_setError("403");
		return (true);
	}
	if (S_ISDIR(buf.st_mode))
	{
		std::vector<std::string>		listOfFiles = listingFile(filename);
		if (_code != "0")
			return (true);
		if (_env.uniqKey.count("index"))
		{
			std::pair<std::string, bool>	index = getIndex(listOfFiles, _env.uniqKey["index"]);
			if (index.second)
				return (_checkFile(filename + "/" + index.first));
		}
		if (_env.uniqKey["auto_index"][0] == "on")
		{
			_code = "200";
			_status = _env.nonUniqKey["return"][_code][0];
			_data = createIndexDir(listOfFiles, _req.getTarget());
			getIndex(listOfFiles, _env.uniqKey["index"]);
			_types = "text/html";
			return (false);
		}
		_setError("404");
		return (true);
	}
	if (_code == "0")
		_code = "200";
	_status = _env.nonUniqKey["return"][_code][0];
	_setType(filename);
	_filename = filename;
	return (false);
}

bool Response::_isCgiFile(std::string root)
{ return (_env.nonUniqKey["cgi"].count(root.substr(root.rfind(".")))); }

std::string	Response::_findCgiPath(std::string root)
{ return (_env.nonUniqKey["cgi"][root.substr(root.rfind("."))][0]); }

void	Response::_execDel(void)
{
	if (!_env.uniqKey.count("root"))
	{
		_setError("404");
		return ;
	}
	struct stat buf;
	std::string filename = _req.getTarget().erase(0, _env.uniqKey["_rootToDel_"][0].length());
	filename = _env.uniqKey["root"][0] + filename;


	stat(filename.c_str(), &buf);
	if (access(filename.c_str(), F_OK) != 0)
	{
		_setError("404");
		return ;
	}
	if (access(filename.c_str(), R_OK) != 0)
	{
		_setError("403");
		return ;
	}
	if (S_ISDIR(buf.st_mode))
	{
		_setError("403");
		return ;
	}
	if (remove(filename.c_str()) == -1)
		throw ResponseError("Response error: remove() failed");
	_code = "200";
	_status = _env.nonUniqKey["return"][_code][0];
	_types = "text/html";
}

void	Response::_execGet(void) {
	if (!_env.uniqKey.count("root") || _env.uniqKey["root"].empty())
	{
		_setError("404");
		return ;
	}
	std::string root = _req.getTarget().erase(0, _env.uniqKey["_rootToDel_"][0].length());
	root = _env.uniqKey["root"][0] + root;
	if (_checkFile(root))
		return ;
	if (_isCgiFile(root))
	{
		std::string cgiPath = _findCgiPath(root);
		CgiHandler	CGI(_env, _req, _types, _clientIp, _sysEnv, *this);
		if (CGI.exec() == -1)
		{
			_setError("500");
			return ;
		}
		_cookies = CGI.getCookie();
		if (CGI.getContentType().empty())
			_types = "text/plain";
		else
			_types = CGI.getContentType();
		if (CGI.getCode().empty())
			_code = "200";
		else
			_code = CGI.getCode();
		if (CGI.getStatus().empty())
			_status = _env.nonUniqKey["return"][_code][0];
		else
			_status = CGI.getStatus();
		if (_code != "200")
			_setError(_code);
		return ;
	}
	_readFile(_filename);
}

void	Response::_readFile(std::string &filename)
{
	std::ifstream file (filename.c_str(), std::ios::binary);
	if (file.bad())
		throw ResponseError("read: error while reading file");
	while (file) 
	{
		char buffer[4096];
		file.read(buffer, 4096);
		_data.insert(_data.end(), buffer, buffer + file.gcount());
	}
	file.close();
}

void	Response::_writeFile(void)
{
	std::string filename = _env.uniqKey["upload"][0] + _req.getUploadFileName();
	std::vector<char> body = _req.getBody();

	std::ofstream	file(filename.c_str());

	if (file.bad())
		throw ResponseError("write: error while write file");
	for (std::vector<char>::iterator it = body.begin(); it != body.end(); it++)
	{
		file << *it;
	}
	return ;
}

void	Response::_uploadFile(void)
{
	if (!_env.uniqKey.count("upload"))
	{
		_setError("404");
		return ;
	}
	struct stat buf;
	std::string filename = _env.uniqKey["upload"][0];
	
	stat(filename.c_str(), &buf);
	if (access(filename.c_str(), F_OK) != 0)
	{
		_setError("404");
		return ;
	}
	if (access(filename.c_str(), W_OK) != 0)
	{
		_setError("403");
		return ;
	}
	if (S_ISDIR(buf.st_mode))
	{
		_writeFile();
		_code = "200";
		_status = _env.nonUniqKey["return"][_code][0];
		_types = "text/html";
		return ;
	}
	_setError("403");
}

void	Response::execute(void) {
	std::string method = _req.getMethod();
	Request::request_type	header = _req.getData();

	if (_req.getBody().size() > static_cast<size_t>(atoi(_env.uniqKey["body_size"][0].c_str())))
	{
		_setError("413");
		return ;
	}
	if (header.count("Content-Length") && atoi(header["Content-Length"][0].c_str()) == 0)
	{
		_setError("413");
		return ;
	}
	if (header.count("Content-Type") && !header["Content-Type"].empty() && !header["Content-Type"][0].empty() && method == "POST" && header["Content-Type"][0] == "multipart/form-data")
	{
		_uploadFile();
		if (_code != "200")
			return ;
	}
	if (std::find(_env.uniqKey["allow_methods"].begin(), _env.uniqKey["allow_methods"].end(), method) != _env.uniqKey["allow_methods"].end())
	{
		(this->*(_methodsFunction.find(method)->second))();
		return ;	
	}
	_setError("405");
	return ;
}

bool	Response::sendData(int clientFd)
{
	int	ret = 0;

	ret = send(clientFd, _data.data(), _data.size(), 0);
	if (ret == -1)
		throw ResponseError("Response error: send() failed");
	_data.erase(_data.begin(), _data.begin() + ret);
	if (_data.empty())
	{
		std::cout << ((_code != "200") ? C_RED : C_GREEN)
				  << '[' << _code << "][" << _status << ']' << C_RESET
				  << " client[" << _clientIp << ']'
				  << " server_name[" << _req.getData()["Host"][0] << ']'
				  << " port[" << _req.getData()["Host"][1] << ']'
				  << " " << _req.getMethod()
				  << " " << _req.getTarget() << std::endl;
	}
	return (_data.empty());
}

bool	Response::_isBinaryFile(std::string filePath)
{
	int	character;
	
	std::ifstream file(filePath.c_str());
	while ((character = file.get()) != EOF && character <= 127)
		;
	if (character == EOF)
		return (false);
	return (true);
}

void	Response::_setType(std::string url)
{
	std::string	extension = url.substr(url.find_last_of("."));
	std::map<std::string, std::string>::const_iterator it = _mimeTypes.find(extension);

	if (it != _mimeTypes.end())
	{
		_types = it->second;
		return ;
	}		
	if (_isBinaryFile(url))
		_types = "application/octet-stream";
	else
		_types = "text/plain";
}

void	Response::constructData(void)
{	
	_header = _req.getVersion() + " " + _code + " " + _status + "\r\n"
        + "Date: " + _getDate() + "\r\n"
        + "Server: webserv/1.0" + "\r\n"
        + "Content-Length: " + to_string(_data.size()) + "\r\n"
        + "Content-Type: " + _types + "\r\n"
        + "Connection: Closed" + "\r\n";

	for (std::vector<std::string>::iterator	it = _cookies.begin(); it != _cookies.end(); it++)
		_header.insert(_header.end(),it->begin(), it->end());
	
    _header += "\n";
	
	_data.insert(_data.begin(), _header.begin(), _header.end());
}

void	Response::setData(std::vector<char> data)
{
	_data = data;
}
