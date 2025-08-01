import bpy

from .EngineConnect import EngineConnect

g_engine = EngineConnect()
g_is_connected = False

class MYADDON_OT_connect_engine(bpy.types.Operator):
    bl_idname = "myaddon.connect_to_engine"
    bl_label = "Connect to Engine"
    bl_options = {'REGISTER', 'UNDO'}

    def execute(self, context):
        global g_engine, g_is_connected
        if g_is_connected:
            self.report({'INFO'}, "Already connected to the engine.")
            return {'CANCELLED'}

        g_engine.connect()
        g_is_connected = True
        return {'FINISHED'}

class MYADDON_OT_disconnect_engine(bpy.types.Operator):
    bl_idname = "myaddon.disconnect_from_engine"
    bl_label = "Disconnect from Engine"
    bl_options = {'REGISTER', 'UNDO'}

    def execute(self, context):
        global g_engine, g_is_connected
        if not g_is_connected:
            self.report({'INFO'}, "Not connected to the engine.")
            return {'CANCELLED'}

        g_engine.disconnect()
        g_is_connected = False
        return {'FINISHED'}