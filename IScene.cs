namespace asciilibur {
    public interface IScene {
        public void OnStart();
        public IScene? OnUpdate(in Renderer renderer, in Input input, float deltaTime);
        public void OnDestroy();
    }
}