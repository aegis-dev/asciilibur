
namespace asciilibur 
{
    public class Context 
    {
        private const long TargetFPS = 60;
        private const long FrameTime = 1000 / TargetFPS;

        private readonly Renderer renderer;
        private readonly Input input;

        public Context(uint width, uint height) 
        {
            if (width > 120 || height > 120) 
            {
                throw new Exception("Window size exceeds maximum of 120 chars width or length");
            }

            if (Console.IsOutputRedirected)
            {
                throw new Exception("Console output is redirected!");
            }

            renderer = new Renderer((int)width, (int)height);
            input = new Input();
        }

        public void Run(in IScene scene) 
        {
            renderer.ClearScreen();

            IScene currentScene = scene;
            currentScene.OnStart();

            long lastTime = DateTime.Now.Ticks / TimeSpan.TicksPerMillisecond;

            while (true) 
            {
                long timeNow = DateTime.Now.Ticks / TimeSpan.TicksPerMillisecond;
                long deltaTime = timeNow - lastTime;
                if (deltaTime < FrameTime)
                {
                    continue;
                }
                lastTime = timeNow;

                IScene? nextScene = currentScene.OnUpdate(renderer, input, (float)deltaTime / 1000.0f);
                if (nextScene != null)
                {
                    currentScene.OnDestroy();
                    currentScene = nextScene;
                    currentScene.OnStart();
                }

                input.PollInput();
            }
        }
    }
}