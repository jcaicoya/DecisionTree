#pragma once

class Configuration
{
public:
    enum class ParallelizationMode { None, Inner, Complete };
    static ParallelizationMode GetParallelizationMode() { return _parellelizationMode; }
    static void SetParallelize(ParallelizationMode parellelizationMode) { _parellelizationMode = parellelizationMode; }
    static bool UseParallelization(size_t numberOfTasks) { return ParallelizationMode::None !=_parellelizationMode && numberOfTasks >= MinimumTasks; }
    static bool UseInnerParallelization(size_t numberOfTasks) { return ParallelizationMode::Inner ==_parellelizationMode && numberOfTasks >= MinimumTasks; }
    static bool UseCompleteParallelization(size_t numberOfTasks) { return ParallelizationMode::Complete == _parellelizationMode && numberOfTasks >= MinimumTasks; }

private:
    inline static ParallelizationMode _parellelizationMode = ParallelizationMode::None;
    static constexpr size_t MinimumTasks = 2;
};