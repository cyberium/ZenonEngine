#pragma once

extern "C" __declspec(dllexport) IznPlugin* WINAPI GetPlugin(std::shared_ptr<IBaseManager> BaseManager);