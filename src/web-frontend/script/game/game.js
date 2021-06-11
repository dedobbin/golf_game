class Game 
{
    static load()
    {
        const script = document.createElement("script");
        script.src = "script/game/index.js";
        document.getElementById("view-game").append(script);
    }
}

export {Game};