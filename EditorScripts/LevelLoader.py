import unreal

def load_scene(file_path):
    world = unreal.EditorLevelLibrary.get_editor_world()
    spawned_actors = {}
    current_actor = None

    with open(file_path, "r") as f:
        lines = f.readlines()

    for line in lines:
        line = line.strip()

        if line.startswith("[GameObject]"):
            current_actor = None

        elif line.startswith("Name: "):
            name = line[6:]
            current_actor = unreal.EditorLevelLibrary.spawn_actor_from_class(unreal.EmptyActor, [0, 0, 0])
            current_actor.set_actor_label(name)
            spawned_actors[name] = current_actor

        elif line.startswith("ParentName: ") and current_actor:
            parent_name = line[12:]
            if parent_name != "None" and parent_name in spawned_actors:
                parent_actor = spawned_actors[parent_name]
                current_actor.attach_to_actor(parent_actor, "None", unreal.AttachmentRule.KEEP_WORLD)

        elif line.startswith("Position: ") and current_actor:
            pos_values = list(map(float, line[9:].split()))
            location = unreal.Vector(pos_values[0], pos_values[1], pos_values[2])
            current_actor.set_actor_location(location)

        elif line.startswith("Rotation: ") and current_actor:
            rot_values = list(map(float, line[10:].split()))
            quat = unreal.Quat(rot_values[0], rot_values[1], rot_values[2], rot_values[3])
            rotator = quat.rotator()
            current_actor.set_actor_rotation(rotator)

        elif line.startswith("Scale: ") and current_actor:
            scale_values = list(map(float, line[7:].split()))
            current_actor.set_actor_scale3d(unreal.Vector(scale_values[0], scale_values[1], scale_values[2]))

        elif line.startswith("MeshType: ") and current_actor:
            mesh_type = line[10:]
            if mesh_type != "None" and mesh_type != "UnknownRenderer":
                mesh_path = "/Game/Meshes/" + mesh_type + "." + mesh_type
                mesh = unreal.EditorAssetLibrary.load_asset(mesh_path)
                if isinstance(mesh, unreal.StaticMesh):
                    smc = unreal.EditorUtilities.create_default_subobject(current_actor, unreal.StaticMeshComponent, "StaticMeshComp")
                    smc.set_editor_property("static_mesh", mesh)
                    current_actor.add_instance_component(smc)
                    smc.register_component()

        elif line.startswith("HasRigidbody: ") and current_actor:
            has_rb = line[14:].lower() == "true"
            if has_rb:
                # Add physics if mesh is present
                components = current_actor.get_components_by_class(unreal.StaticMeshComponent)
                if components:
                    mesh_comp = components[0]
                    mesh_comp.set_editor_property("simulate_physics", True)

    unreal.EditorLevelLibrary.save_current_level()
    unreal.log("Scene loaded successfully from: " + file_path)


# === Usage Example ===

level_file_path = "D:\GitRepository\GDENG03\Scenes\TestCase9.level"
load_scene(level_file_path)
