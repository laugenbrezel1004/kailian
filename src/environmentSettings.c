#include "../include/environmentSettings.h"

const char *configPath = "/etc/kailian/kailian.conf";

const char *keys[] = {"name",
                      "endpoint_generate",
                      "endpoint_running_model",
                      "endpoint_info",
                      "endpoint_ollama_version",
                      "endpoint_chat",
                      "endpoint_show",
                      "system"};

const char *templateConfig =
    "name = chatgpt\n"
    "endpoint_generate = http://localhost:11434/api/generate\n"
    "endpoint_info = http://localhost:11434/api/tags\n"
    "endpoint_running_model = http://localhost:11434/api/ps\n"
    "endpoint_ollama_version = http://localhost:11434/api/version\n"
    "endpoint_chat = http://localhost:11434/api/chat\n"
    "endpoint_show = http://localhost:11434/api/show\n"
    "system = \"Do not answer in markdown\"";
