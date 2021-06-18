import {Level} from '../level.js'; 
import {Entity} from '../entity.js'; 
import {isEqual} from '../utils.js'
import {Palette, CursorModeEnum} from './palette.js'; 
import {EntityProperties} from './entity_properties.js'

let currentLevel = null;

class LevelField
{
    static initCanvas()
    {
        let x, y;
        function findxy(e) {
            x = e.clientX - canvas.offsetLeft;
            y = e.clientY - canvas.offsetTop;		
            
            let elem = canvas;
            while(elem){
                x += elem.scrollLeft;	
                y += elem.scrollTop;
                elem = elem.parentElement;
            }
        }
        const canvas = document.getElementById('level-editor-canvas');
        
        canvas.addEventListener("mousemove", function (e) {
            findxy(e)
        }, false);
        canvas.addEventListener("mousedown", function (e) {
            findxy(e);
            if (Palette.getCursorMode() == CursorModeEnum.DRAW){
                const selectedEntity = Palette.getSelectedEntity();
                currentLevel.addEntity(selectedEntity, x, y);
            } else if (Palette.getCursorMode() == CursorModeEnum.ERASE){
                currentLevel.eraseEntity(x, y);
            } else if (Palette.getCursorMode() == CursorModeEnum.SELECT){
                const selected = currentLevel.getEntity(x, y);
                if (selected){
                    EntityProperties.display(selected);
                }
            }
        }, false);
        canvas.addEventListener("mouseup", function (e) {
            findxy(e)
        }, false);
        canvas.addEventListener("mouseout", function (e) {
            findxy(e)
        }, false);
    }


    //Fetches data to display on canvas + palette
    static loadLevel(data) 
    {
        if (!data){
            fetch('level_data.json')
            .then(response => {
                if (response.status != 404){
                    response.json().then(json=>{
                        LevelField.loadLevel(json);
                    });
                } else {
                    console.log("Default level file not found");
                }
            });
            return;
        }
        

        currentLevel = new Level(document.getElementById("level-editor-canvas").getContext("2d"), data.name, data.world_w, data.world_h, data.world_gravity); 
    
        const seenEntities = [];
        data.entities.forEach(rawEntity=>{
            const rawEntityCopy = JSON.parse(JSON.stringify(rawEntity));
            const x = rawEntityCopy.pos[0];
            const y = rawEntityCopy.pos[1];
            const w = rawEntityCopy.pos[2];
            const h = rawEntityCopy.pos[3];
            delete rawEntityCopy.pos;
            rawEntityCopy.size = {w: w, h: h};
    
            /** check if already if duplicate */
            let sameAs = null;
            for (let i = 0; i < seenEntities.length;i++){
                if (isEqual(seenEntities[i].fields, rawEntityCopy)){
                    sameAs = seenEntities[i];
                    break;
                }
            }
            if (!sameAs){
                sameAs  = Palette.entityExists(rawEntityCopy);
            }
            /** end check if duplicate **/

            let entity = null;
            
            if (sameAs){
                entity = sameAs;
            } else {
                entity = new Entity();
                entity.fields = rawEntityCopy;
                seenEntities.push(entity);
                Palette.addEntity(entity);
            }

            currentLevel.addEntity(entity, x, y);
        });
        Palette.refresh();
    }

    static clear()
    {
        if (!currentLevel){
			alert("No level loaded");
			return;
		}

		if (confirm("Clear all entities from field?")){
			currentLevel.clear();
		}
    }

    static refresh()
    {
        currentLevel.refreshCanvas();
    }

    static import()
    {
        if (currentLevel){
            if (!confirm("This will overwrite current level data, continue?")){
                return;
            }
        }
        const input = document.createElement('input');
		input.type = 'file';
		
		input.onchange = e => { 
			const file = e.target.files[0];
			file.text().then(content=>{
				LevelField.loadLevel(JSON.parse(content));
			});
		}
		
		input.click();
    }

    static export()
    {
        if (!currentLevel){
			alert("No level loaded");
			return;
		}

		const json = currentLevel.toJSON();

		const element = document.createElement('a');
		element.setAttribute('href', 'data:application/json;charset=utf-8,' + encodeURIComponent(json));
		element.setAttribute('download', "level.json");
		element.style.display = 'none';
		document.body.appendChild(element);
		element.click();
		document.body.removeChild(element);
    }

    static flipGrid(forceState = null)
    {
        if (!currentLevel){
            alert("No level loaded");
            return;
        }

        if (forceState === true || forceState === false){
            currentLevel.displayGrid = forceState;
        } else {
            currentLevel.displayGrid = !currentLevel.displayGrid; 
        }

        currentLevel.refreshCanvas();

        const elem = document.getElementById("grid-control");
        if (currentLevel.displayGrid){
            elem.innerText = "grid off";
        } else {
            elem.innerText = "grid on";
        }
    }
}

export {LevelField}