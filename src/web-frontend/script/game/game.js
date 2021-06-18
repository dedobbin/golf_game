class Game 
{
    static load(_parent)
    {
        parent = _parent ?? document.getElementById("view-game");
        //const parent = document.getElementById("view-game");
        const script = document.createElement("script");
        script.src = "script/game/index.js";
        parent.append(script);
    }
}

export {Game};