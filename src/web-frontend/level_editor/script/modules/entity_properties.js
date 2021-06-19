import {Palette} from './palette.js'; 
import {LevelField} from './level_field.js'

class EntityProperties
{
    static display(entity)
    {
        const prevSelectedEntity = Palette.getSelectedEntity();
        
        if (entity == prevSelectedEntity){
            return;
        }
        
        if (!EntityProperties.confirmContinue(prevSelectedEntity)){
            return;
        }

        EntityProperties.enable();
        const elem = document.getElementById("entity-fields");

        elem.innerHTML = "";

        Object.keys(entity.fields).forEach(key =>{
            EntityProperties.addNew(key, entity.fields[key]);
        });

        elem.setAttribute("data-entity-id", entity.id);
        

        if (prevSelectedEntity) {
            prevSelectedEntity.isSelected = false;
        }
        entity.isSelected = true;
        Palette.refresh();
    }

    static enable()
    {
        Array.from(document.querySelectorAll("#entity-properties-control button, #entity-properties-control input, #entity-properties button")).forEach(e=>{
            e.removeAttribute("disabled");
        });
    }

    static disable()
    {
        document.getElementById("entity-fields").innerHTML = "";

        Array.from(document.querySelectorAll("#entity-properties-control button, #entity-properties-control input, #entity-properties button")).forEach(e=>{
            e.setAttribute("disabled", 1);
        });
    }

    static save()
    {
        if (!EntityProperties.sanityCheck()){
            return;
        }
    
        const elem = document.getElementById("entity-fields");
        const entity = Palette.getEntity(elem.getAttribute("data-entity-id"));
    
        if (!entity){
            console.error("No entity " + elem.getAttribute("data-entity-id") + " in entity memory, aborting..");
            return;
        }
    
        const rows = elem.children;
        entity.fields = EntityProperties.toNestedObject(rows);
    
        console.log("saved: ", entity);
    
        LevelField.refresh();
        Palette.refresh();
    }

    static addNew(key, value)
    {
        if (typeof value === 'object' && value !== null){
            if (Object.entries(value).length == 0){
                //Display empty objects as empty string
                //console.log("DEBUG addNewPropertyField, empty object: ", key, ":", value)
                EntityProperties.addNew(key, "");	
            } else {
                Object.keys(value).forEach(new_key=>{
                    EntityProperties.addNew(key + '.' + new_key, value[new_key]);	
                });
            }
            return;
        }
        const label = document.createElement("input");
        label.classList.add("entity-property-key");
        label.type = "text";
        label.value = key;
        //label.name = key + "-key";
        label.style.width="70%";

        const inputField = document.createElement("input");
        inputField.classList.add("entity-property-value");
        inputField.type = "text";
        inputField.name = key;
        inputField.style.width="20%";
        //inputField.value = entity.fields[key];
        inputField.value = value;

        const delBtn = document.createElement("button");
        delBtn.innerText = "X";
        delBtn.addEventListener("click", e=>{
            delBtn.parentNode.parentNode.removeChild(delBtn.parentNode);
        });

        const row = document.createElement("div");
        row.classList.add("row");
        if (key.includes("graphic.")){
            row.classList.add("graphic-frame-data")
        }
        row.appendChild(label);
        row.appendChild(inputField);
        row.appendChild(delBtn);
        row.appendChild(document.createElement("hr"));

        const elem = document.getElementById("entity-fields")
        elem.appendChild(row);
    }

    static validate()
    {
        if(!EntityProperties.sanityCheck()){
            return;
        }
    
        //TODO: check if would work in game
    
        alert("Sane entity");
    }

    static sanityCheck()
    {
        const elem = document.getElementById("entity-fields");
        const children = elem.children;
        const seenValues = [];
        for (let i = 0; i < children.length; i++){
            const value = children[i].getElementsByClassName("entity-property-key")[0].value;
            if (value == ""){
                alert("Cannot save entity, one of attributes is unnamed..");
                return false;
            }
            if (seenValues.includes(value)){
                alert("Cannot save entity, '" + value + "' is set multiple times");
                return false;
            }

            seenValues.push(value);
        }

        const mandatoryFields = ["name", "type", "size.w", "size.h"];
        let missingMandatoryFields = mandatoryFields.filter(e=>{
            return !seenValues.includes(e);
        })

        if (missingMandatoryFields.length){
            alert("Missing mandatory fields: "+ missingMandatoryFields.join(", "));
            return false;
        }

        return true;
    }

    static toNestedObject(rows)
    {
        if (!rows){
            rows = document.getElementById("entity-fields").children;
        }

        const result = {};
        for (let i = 0; i < rows.length; i++){
            let key = rows[i].querySelector(".entity-property-key").value;
            let value = rows[i].querySelector(".entity-property-value").value;

            const setAttr = (owner, key) => {
                const split = key.indexOf(".");
                if (split == -1){
                    owner[key] = value;
                } else {
                    const first = key.substring(0, split);
                    const last = key.substring(split + 1, key.length);//overshoots with second param but doesn't matter, it needs entire til end anyway
                    if (owner[first] == undefined || owner[first] == ""){ //empty string check because empty objects are 'displayed' as ""
                        owner[first] = {};
                    }

                    if (last.includes(".")){
                        setAttr(owner[first], last)
                    } else {
                        console.log("DEBUG setting ", last ," to ", value);
                        owner[first][last] = value;
                    }
                }
            }
            setAttr(result, key);
        }

        const _fixTypes =  input =>{

            const _parse = obj => 
            {
                for (var k in obj)
                {
                    if (obj[k] === ""){
                        obj[k] = {};
                    }
                    else if (typeof obj[k] == "object" && obj[k] !== null){
                        let shouldBeArray = true;
                        //object with all keys as ints should be array
                        Object.keys(obj[k]).forEach(innerK =>{
                            if (isNaN(parseInt(innerK)))
                                shouldBeArray = false;
                        })
                        if (shouldBeArray){
                            obj[k] = Object.values(obj[k]);
                        } 
                        _parse(obj[k]);
                    }else if (typeof obj[k] == "string"){
                        if (!isNaN(+obj[k])){
                            if (obj[k].includes(".")){
                                obj[k] = parseFloat(obj[k]);
                            } else {
                                obj[k] = parseInt(obj[k]);
                            }
                        } else if (obj[k] == "true" || obj[k] == "false"){
                            obj[k] = obj[k] === "true"
                        }
                    } 
                }
            }
            _parse(input);
        }

        _fixTypes(result);


        return result;
    }

    static hideGraphicInfo(checked)
    {
        if (checked){
			document.getElementById("entity-properties").classList.add("hide-graphic-data");
		} else {
			document.getElementById("entity-properties").classList.remove("hide-graphic-data");
		}
    }

    static confirmContinue(entity){
        //Nags if current selected entity is dirty
        const prevSelectedEntity = entity ?? Palette.getSelectedEntity();
        if (prevSelectedEntity && prevSelectedEntity.isDirty(EntityProperties.toNestedObject())){
            return confirm("Entity was not saved, continue will lose data");
        }
    
        return true;
    }
}

export {EntityProperties}