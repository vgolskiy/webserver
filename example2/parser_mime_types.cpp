/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_mime_types.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbarbera <fbarbera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/12 13:20:40 by fbarbera          #+#    #+#             */
/*   Updated: 2021/03/21 15:59:17 by fbarbera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_config.hpp"


std::map<std::string, std::string> gen_mime()
{
	std::string s1 = ".aac .abw .arc .avi .azw .bin .bmp .bz .bz2 .csh .css .csv .doc .docx .eot .epub .gz .gif .htm .html .ico .ics .jar .jpeg .jpg .js .json .jsonld .mid .midi .mjs .mp3 .mpeg .mpkg .odp .ods .odt .oga .ogv .ogx .opus .otf .png .pdf .php .ppt .pptx .rar .rtf .sh .svg .swf .tar .tif .tiff .ts .ttf .txt .vsd .wav .weba .webm .webp .woff .woff2 .xhtml .xls .xlsx .xml .xul .zip .3gp .3g2 .7z";
	std::string s2 = "audio/aac application/x-abiword application/x-freearc video/x-msvideo application/vnd.amazon.ebook application/octet-stream image/bmp application/x-bzip application/x-bzip2 application/x-csh text/css text/csv /msword application/vnd.openxmlformats-officedocument.wordprocessingml.document application/vnd.ms-fontobject application/epub+zip application/gzip image/gif text/html text/html image/vnd.microsoft.icon text/calendar application/java-archive image/jpeg image/jpeg text/javascript application/json application/ld+json audio/midi audio/x-midi text/javascript audio/mpeg video/mpeg application/vnd.apple.installer+xml application/vnd.oasis.opendocument.presentation application/vnd.oasis.opendocument.spreadsheet application/vnd.oasis.opendocument.text audio/ogg video/ogg application/ogg audio/opus font/otf image/png application/pdf application/x-httpd-php application/vnd.ms-powerpoint application/vnd.openxmlformats-officedocument.presentationml.presentation application/vnd.rar application/rtf application/x-sh image/svg+xml application/x-shockwave-flash application/x-tar image/tiff image/tiff video/mp2t font/ttf text/plain application/vnd.visio audio/wav audio/webm video/webm image/webp font/woff font/woff2 application/xhtml+xml application/vnd.ms-excel application/vnd.openxmlformats-officedocument.spreadsheetml.sheet application/xml application/vnd.mozilla.xul+xml application/zip video/3gpp video/3gpp2 application/x-7z-compressed"; //не надо стесняться
	std::vector<std::string> v1 = ft_split_string_to_vector(s1, ' ');
	std::vector<std::string> v2 = ft_split_string_to_vector(s2, ' ');
	std::map<std::string, std::string> m;
	for (size_t i = 0; i < v1.size(); i++)
	{
		m.insert(std::pair<std::string, std::string>(v1[i], v2[i]));
	}
	return m;
}