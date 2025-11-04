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
#include "../../Includes/CommandHandler.hpp"

void Channel::executeMode(Server &server, Client* c, const std::string& mode, const std::string& param) {
    std::cout << "Debugging: executeMode called by client " << c->getNick() << " on channel " << _name << std::endl;

    if (!isOperator(c)) {
        c->sendNumericReply(server, 482, _name, "You're not a channel operator");
        return;
    }

    bool addMode = true;
    std::stringstream paramStream(param);
    std::string currentParam;

    for (size_t i = 0; i < mode.size(); ++i) {
        char modeChar = mode[i];

        if (modeChar == '+') {
            addMode = true;
            continue;
        } else if (modeChar == '-') {
            addMode = false;
            continue;
        }

        switch (modeChar) {
            case 'i':
                _mode_invite_only = addMode;
                break;

            case 't':
                _mode_topic_ops_only = addMode;
                break;

            case 'k':
                if (addMode) {
                    if (paramStream >> currentParam) {
                        _key = currentParam;
                    } else {
                        c->sendNumericReply(server, 461, "MODE", "Not enough parameters for +k");
                        return;
                    }
                } else {
                    _key.clear();
                }
                break;

            case 'l':
                if (addMode) {
                    if (paramStream >> currentParam) {
                        _limit = std::atoi(currentParam.c_str());
                    } else {
                        c->sendNumericReply(server, 461, "MODE", "Not enough parameters for +l");
                        return;
                    }
                } else {
                    _limit = 0;
                }
                break;
			case 'o' :
				if (addMode){
					if (paramStream >> currentParam){
						Client* targetClient = server.findClientByNick(currentParam);
						if(!targetClient){
							std::cout << "Debugging : Target client not found for +o mode" << currentParam << std::endl;
							c->sendNumericReply(server,401,currentParam,"No such nick/channel");
							return;
						}

                        // Prevent the bot from being given +o mode
                        if (targetClient->getNick() == "bot") {
                        c->sendNumericReply(server, 482, _name, "Cannot give operator privileges to the bot");
                        return;
                        }
						
						addOperator(targetClient);
						std::cout << "Debugging Granted operator status to " << currentParam << std::endl;

						std::string modeMessage = formatMessage(
							c->getNick() + "!" + c->getUser() + "@" + c->getHost(),
							"MODE",
							_name + " +o " + currentParam,
							""
						);
						notifyMembers(server, modeMessage);
					} else {
						std::cout << "Debugging : Not enough parameters for +o mode" << std::endl;
						c->sendNumericReply(server,461,"MODE","Not enough parameters for +o");
						return;
					}
				} else {
					if (paramStream >> currentParam) {
						Client* targetClient = server.findClientByNick(currentParam);
						if(!targetClient){
							std::cout << "Debugging : Target client not found for -o mode" << currentParam << std::endl;
							c->sendNumericReply(server,401,currentParam,"No such nick/channel");
							return;
						}

						removeOperator(targetClient);
						std::cout << "Debugging Revoked operator status from " << currentParam << std::endl;

						std::string modeMessage = formatMessage(
							c->getNick() + "!" + c->getUser() + "@" + c->getHost(),
							"MODE",
							_name + " -o " + currentParam,
							""
						);
						notifyMembers(server, modeMessage);
					} else {
						std::cout << "Debugging : Not enough parameters for -o mode" << std::endl;
						c->sendNumericReply(server,461,"MODE","Not enough parameters for -o");
						return;
					}
				}
				break;

            default:
                c->sendNumericReply(server, 501, std::string(1, modeChar), "Unknown mode flag");
                return;
        }
    }

    // Notify channel members of the mode change
    notifyMembers(server, formatMessage(
        c->getNick() + "!" + c->getUser() + "@" + c->getHost(),
        "MODE",
        _name + " " + mode,
        param
    ));

    // Send confirmation to the client
    c->sendNumericReply(server, 324, _name, "+" + mode + " " + param);
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


