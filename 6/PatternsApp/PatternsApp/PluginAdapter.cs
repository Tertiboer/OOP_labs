
namespace PatternsApp
{
    // Adapter pattern: adapts LegacyPlugin to IPlugin
    public class PluginAdapter : IPlugin
    {
        private LegacyPlugin _legacy;

        public PluginAdapter(LegacyPlugin legacy)
        {
            _legacy = legacy;
        }

        public void Execute()
        {
            _legacy.Run();
        }
    }
}
