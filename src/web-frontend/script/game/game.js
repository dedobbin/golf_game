class Game 
{
    static load()
    {
        fetch("script/game/view-game-body-html.txt").then(response=>{
            response.text().then(content=>{
                console.log(content)
                const div = document.createElement("div");
                div.innerHTML = content;
                document.getElementById("view-game").appendChild(div);
            })
        })
    }
}

export {Game};