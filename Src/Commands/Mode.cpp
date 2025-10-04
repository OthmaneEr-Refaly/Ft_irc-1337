/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:37:42 by mobouifr          #+#    #+#             */
/*   Updated: 2025/09/24 09:33:27 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Headers.hpp"

void Channel::executeMode(Server &server, Client* c, const std::string& mode, const std::string& param) {
    std::cout << "Debugging: executeMode called by client " << c->getNick() << " on channel " << _name << std::endl;

    if (!isOperator(c)) {
        c->sendNumericReply(server, 482, _name, "You're not a channel operator");
        return;
    }

    bool addMode = true;
    std::stringstream paramStream(param);
    std::string currentParam;

    std::cout << "Debugging: paramStream contents: '" << param << "'" << std::endl;

    for (size_t i = 0; i < mode.size(); ++i) {
        char modeChar = mode[i];

        if (modeChar == '+') {
            addMode = true;
            continue;
        } else if (modeChar == '-') {
            addMode = false;
            continue;
        }

        std::cout << "Debugging: Processing mode flag '" << modeChar << "' with addMode=" << addMode << std::endl;

        switch (modeChar) {
            case 'i':
                _mode_invite_only = addMode;
                std::cout << "Debugging: Invite-only mode set to " << _mode_invite_only << std::endl;
                break;

            case 't':
                _mode_topic_ops_only = addMode;
                std::cout << "Debugging: Topic-ops-only mode set to " << _mode_topic_ops_only << std::endl;
                break;

            case 'k':
                if (addMode) {
                    if (paramStream >> currentParam) {
                        std::cout << "Debugging: Extracted parameter for +k: '" << currentParam << "'" << std::endl;
                        _key = currentParam;
                    } else {
                        c->sendNumericReply(server, 461, "MODE", "Not enough parameters for +k");
                    }
                } else {
                    _key.clear();
                }
                break;

            case 'l':
                if (addMode) {
                    if (paramStream >> currentParam) {
                        std::cout << "Debugging: Extracted parameter for +l: '" << currentParam << "'" << std::endl;
                        _limit = std::atoi(currentParam.c_str());
                    } else {
                        c->sendNumericReply(server, 461, "MODE", "Not enough parameters for +l");
                    }
                } else {
                    _limit = 0;
                }
                break;

            default:
                c->sendNumericReply(server, 501, std::string(1, modeChar), "Unknown mode flag");
                break;
        }
    }

    notifyMembers(server, ":" + c->getNick() + " MODE " + _name + " " + mode + " " + param);
}

void handleMode(Server &server, Client &client, const Command &cmd) {
    if (cmd.params.size() < 2) {
        client.sendNumericReply(server, ERR_NEEDMOREPARAMS, "MODE", "Not enough parameters");
        return;
    }

    std::string channelName = cmd.params[0];
    Channel* channel = server.getChannel(channelName);
    if (!channel) {
        client.sendNumericReply(server, 403, channelName, "No such channel");
        return;
    }

    std::string mode = cmd.params[1];

    // Concatenate all parameters after the mode string
    std::string param;
    for (size_t i = 2; i < cmd.params.size(); ++i) {
        if (!param.empty()) {
            param += " ";
        }
        param += cmd.params[i];
    }

    std::cout << "Debugging: param string passed to executeMode: '" << param << "'" << std::endl;

    channel->executeMode(server, &client, mode, param);
}
