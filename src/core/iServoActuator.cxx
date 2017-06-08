#include <iterator>

#include "iServoActuator.hpp"
#include "external/json/json.hpp"
#include <iostream>

using namespace upm;

iServoActuator::iServoActuator ()
{
    DEBUG_MSG("XXX");
    AddJsonDeserializer(this, &_DeserializeAngleCommands);
}

iServoActuator::~iServoActuator ()
{
    DEBUG_MSG("XXX");
}

void iServoActuator::AngleAll (float angle)
{
    std::map<std::string, float> tmpcmds;

    for(std::vector<std::string>::const_iterator it = Commands().begin();
            it != Commands().end(); ++it)
        tmpcmds[*it] = angle;

    AngleForCommands(tmpcmds);
}

void iServoActuator::AngleForCommand (float angle, std::string command)
{
    DEBUG_MSG("Number of commands: " << Commands().size() << std::endl);

    if (std::find(Commands().begin(), Commands().end(), command) == Commands().end())
    {
        std::stringstream ss;
        ss << DerivedClassName() << " does not provide command: '"
            << command << "'.  Valid commands are: {";
        std::copy(Commands().begin(), Commands().end() - 1,
                std::ostream_iterator<std::string>(ss, ", "));
        if (!Commands().empty())
            ss << Commands().back();
        ss << "}";
        throw std::invalid_argument(ss.str());
    }

    AngleForCommands(std::map<std::string, float> {{command, angle}});
}

void iServoActuator::AngleFromJson(std::string json_commands)
{
    std::vector<ActuatorCommand<float>> cmds;

    /* First, try to deserialize 1 command */
    try
    {
        nlohmann::json json_cmd = nlohmann::json::parse(json_commands);
        cmds.push_back(json_cmd);
    }
    catch (const std::exception& e)
    {
        DEBUG_MSG("Deserialize single command failed, attempting to deserialize command vector...");
    }

    /* If no commands were created, try to deserialize a vector of commands */
    if (cmds.empty())
    {
        try
        {
            nlohmann::json json_cmd = nlohmann::json::parse(json_commands);
            /* TODO: get rid of this second copy */
            std::vector<ActuatorCommand<float>> tmp_cmds = json_cmd;
            cmds = tmp_cmds;
        }
        catch (const std::exception& e)
        {
            DEBUG_MSG("Deserialize command vector failed...");
        }
    }

    /* If no commands exist, throw */
    if (cmds.empty())
    {
        std::stringstream ss;
        ss << "Failed to deserialize commands from json string: '";
        ss << json_commands << "'" << std::endl;
        ss << "Valid JSON command string examples:" << std::endl;

        /* Make the examples (at least 2) */
        std::vector<ActuatorCommand<float>> tmp_cmds(Commands().size() < 2 ? 2 : Commands().size());
        for(size_t i = 0; i < Commands().size(); i++)
            tmp_cmds[i].command = Commands()[i];

        /* Example strings show a single command and vector or commands */
        nlohmann::json jcmd = tmp_cmds[0];
        nlohmann::json jcmds = tmp_cmds;
        ss << "'" << jcmd << "' or '" << jcmds << "'";
        throw std::invalid_argument(ss.str());
    }

    /* Run the commands */
    for (std::vector<ActuatorCommand<float>>::const_iterator it = cmds.begin(); it != cmds.end(); ++it)
    {
        AngleForCommand(it->value, it->command);
    }
}

std::string iServoActuator::_DeserializeAngleCommands (iUpmObject * inst)
{
    /* Downcast to reference (throws if cast fails) */
    iServoActuator& ref = dynamic_cast<iServoActuator&>(*inst);

    /* Make sure the derived class name (from std::type_info) matches a JSON sensor class name */
    std::string classname_derived = ref.DerivedClassName();

    /* First, get a json reference to the "Sensor Class" */
    nlohmann::json json_class = ref.LibraryJson()["Sensor Class"];

    if (json_class.find(classname_derived) == json_class.end())
    {
        std::stringstream ss;
        for (nlohmann::json::iterator it = json_class.begin(); it != json_class.end(); ++it)
        {
            ss << it.key();
            if (*it != json_class.back()) ss << ", ";
        }

        throw std::invalid_argument("JSON library descriptor file " + inst->LibraryJsonFilename() + " does not contain a class object named '" +
                classname_derived + "'.  Valid classes are: {" + ss.str() + "}");
    }

    /* Get a reference to the 'Commands' object, if none exists, catch and continue */
    nlohmann::json jsrs;
    try { jsrs = ref.LibraryJson()["Sensor Class"][classname_derived].at("Commands"); }
    catch (...) {}

    /* Add all commands to iSensorType's command vector */
    for (nlohmann::json::iterator it = jsrs.begin(); it != jsrs.end(); ++it)
    {
        upm::ActuatorSink<float> _as = it.value();
        ref.AddCommand(it.key(), _as.unit);
    }

    /* Return a string */
    return jsrs.dump();
}
