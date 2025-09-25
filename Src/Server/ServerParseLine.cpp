/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerParseLine.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 10:51:31 by mobouifr          #+#    #+#             */
/*   Updated: 2025/09/25 10:52:04 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Headers.hpp"

Command Server::parseRawLine(const std::string &line)
{
    Command cmd;
    size_t pos = 0;
    size_t len = line.size();

    // ===== STEP 1: Parse Prefix (optional) =====
    if (pos < len && line[pos] == ':') {
        size_t spacePos = line.find(' ', pos);
        if (spacePos == std::string::npos) {
            return cmd; // malformed line
        }
        cmd.prefix = line.substr(pos + 1, spacePos - pos - 1);
        pos = spacePos + 1;
    }
	std::cout << "this is the prefix" << cmd.prefix << std::endl;
    // Skip spaces after prefix
    while (pos < len && line[pos] == ' ') 
        pos++;

    // ===== STEP 2: Parse Command (required) =====
    size_t spacePos = line.find(' ', pos);
    if (spacePos == std::string::npos) {
        cmd.command = line.substr(pos);
		pos = std::string::npos;
    } else {
        cmd.command = line.substr(pos, spacePos - pos);
        pos = spacePos + 1;
    }
    // Convert command to uppercase
    for (size_t i = 0; i < cmd.command.size(); i++) {
        cmd.command[i] = std::toupper(cmd.command[i]);
    }
	std::cout << "this is the command " << cmd.command << std::endl;

    // ===== STEP 3: Parse Parameters =====
    // Skip spaces before parameters
    while (pos < len && line[pos] == ' ') 
        pos++;
    // Check if no parameters remain
    if (pos >= len || pos == 0) {
        cmd.params.clear();
        return cmd;
    }

    // Parse all parameters
    while (pos < len) {
        // Handle trailing parameter (starts with ':')
        if (line[pos] == ':') {
            cmd.params.push_back(line.substr(pos + 1));
            break;
        }
        // Handle regular parameter
        size_t nextSpace = line.find(' ', pos);
        if (nextSpace == std::string::npos) {
            // Last parameter
            cmd.params.push_back(line.substr(pos));
            break;
        } else {
            // Middle parameter
            cmd.params.push_back(line.substr(pos, nextSpace - pos));
            pos = nextSpace + 1;
            // Skip multiple spaces
            while (pos < len && line[pos] == ' ') 
                pos++;
        }
    }

    // Debug output for parameters
    for (size_t i = 0; i < cmd.params.size(); i++) {
        std::cout << "this is the param[" << i << "] = " << cmd.params[i] << std::endl;
    }
    return cmd;
}
