#pragma once

class Configuration
{
public:
    static bool GetParallelize() { return _parellelize; }
    static void SetParallelize(bool parellelize) { _parellelize = parellelize; }
    static bool Parallelize(size_t numberOfTasks) { return _parellelize && numberOfTasks >= MinimumTasks; }

private:
    inline static bool _parellelize = false;
    static constexpr size_t MinimumTasks = 2;
};