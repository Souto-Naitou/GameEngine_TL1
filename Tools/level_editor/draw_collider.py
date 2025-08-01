import bpy
import mathutils
import gpu
import gpu_extras.batch

class DrawCollider:
    # 描画ハンドル
    handle = None

    # 3Dビューに登録する描画関数
    def draw_collider():
        vertices = {"pos":[]}
        indices = []

        # 各頂点の、オブジェクト中心からのオフセット
        offset = 0.5
        offsets = [
            [-offset, -offset, -offset], # 左下前
            [+offset, -offset, -offset], # 右下前
            [-offset, +offset, -offset], # 左上前
            [+offset, +offset, -offset], # 右上前
            [-offset, -offset, +offset], # 左下奥
            [+offset, -offset, +offset], # 右下奥
            [-offset, +offset, +offset], # 左上奥
            [+offset, +offset, +offset], # 右上奥
        ]

        # 立方体のX, Y, Z方向サイズ
        size = [2,2,2]

        # 現在シーンのオブジェクトリストを走査
        for object in bpy.context.scene.objects:
            # コライダープロパティがなければ描画をスキップ
            if not "collider" in object:
                continue

            # 中心点、サイズの変数を宣言
            center = mathutils.Vector((0,0,0))
            size = mathutils.Vector((2,2,2))

            # プロパティから値を取得
            for i in range(3):
                center[i]=object["collider_center"][i]
                size[i]=object["collider_size"][i]

            # 追加前の頂点数
            start = len(vertices["pos"])
            # Boxの8頂点分回す
            for ofs in offsets:
                pos = center.copy()
                # 中心点を基準に各頂点ごとにずらす
                for i in range(3):
                    pos[i] += ofs[i] * size[i]
                # ローカル座標からワールド座標に変換
                pos = object.matrix_world @ pos
                # 頂点データリストに座標を追加
                vertices['pos'].append(pos)
            
            # 前面を構成する辺の頂点インデックス
            indices.append([start+0, start+1])
            indices.append([start+2, start+3])
            indices.append([start+0, start+2])
            indices.append([start+1, start+3])
            # 奥面を構成する辺の頂点インデックス
            indices.append([start+4, start+5])
            indices.append([start+6, start+7])
            indices.append([start+4, start+6])
            indices.append([start+5, start+7])
            # 前と頂点をつなぐ辺の頂点インデックス
            indices.append([start+0, start+4])
            indices.append([start+1, start+5])
            indices.append([start+2, start+6])
            indices.append([start+3, start+7])

        # Blender4.4では"3D_UNIFORM_COLOR"が使用できないため、"UNIFORM_COLOR"を使用する
        shader = gpu.shader.from_builtin("UNIFORM_COLOR")

        batch = gpu_extras.batch.batch_for_shader(shader, 'LINES', vertices, indices=indices)

        color = [0.5, 1.0, 1.0, 1.0]
        shader.bind()
        shader.uniform_float("color", color)
        # 描画
        batch.draw(shader)