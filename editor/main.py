import tkinter as tk
from tkinter import filedialog, messagebox, simpledialog


EMPTY = 0x00
WALL = 0x01

GRID_COLOR = "#808080"
EMPTY_COLOR = "#ffffff"
WALL_COLOR = "#444444"


class MapEditor:
        def __init__(self, root):
                self.root = root

                self.root.title("Map Editor")
                self.root.geometry("1200x800")

                self.width = 0
                self.height = 0
                self.tiles = []
                self.filename = None

                self.painting = False
                self.erasing = False

                self.create_menu()
                self.create_ui()

                self.root.bind("<Control-n>", self.create_map)
                self.root.bind("<Control-o>", self.open_map)
                self.root.bind("<Control-s>", self.save_map)
                self.root.bind("<Control-Shift-S>", self.save_map_as)
                self.root.bind("<Control-q>", self.exit_editor)
                self.root.bind("<Escape>", self.exit_editor)

        def create_menu(self):
                menu_bar = tk.Menu(self.root)

                file_menu = tk.Menu(
                        menu_bar,
                        tearoff=False,
                )

                file_menu.add_command(
                        label="New",
                        accelerator="Ctrl+N",
                        command=self.create_map,
                )

                file_menu.add_command(
                        label="Open",
                        accelerator="Ctrl+O",
                        command=self.open_map,
                )

                file_menu.add_separator()

                file_menu.add_command(
                        label="Save",
                        accelerator="Ctrl+S",
                        command=self.save_map,
                )

                file_menu.add_command(
                        label="Save As...",
                        accelerator="Ctrl+Shift+S",
                        command=self.save_map_as,
                )

                file_menu.add_separator()

                file_menu.add_command(
                        label="Exit",
                        accelerator="Ctrl+Q",
                        command=self.exit_editor,
                )

                menu_bar.add_cascade(
                        label="File",
                        menu=file_menu,
                )

                edit_menu = tk.Menu(
                        menu_bar,
                        tearoff=False,
                )

                edit_menu.add_command(
                        label="Grid Size...",
                        command=self.change_grid_size,
                )

                menu_bar.add_cascade(
                        label="Edit",
                        menu=edit_menu,
                )

                self.root.config(menu=menu_bar)

        def create_ui(self):
                self.canvas = tk.Canvas(
                        self.root,
                        bg=EMPTY_COLOR,
                        highlightthickness=0,
                )

                self.canvas.pack(
                        fill=tk.BOTH,
                        expand=True,
                )

                self.canvas.bind(
                        "<Button-1>",
                        self.click_tile,
                )

                self.canvas.bind(
                        "<B1-Motion>",
                        self.drag_tile,
                )

                self.canvas.bind(
                        "<ButtonRelease-1>",
                        self.stop_painting,
                )

                self.canvas.bind(
                        "<Button-3>",
                        self.right_click_tile,
                )

                self.canvas.bind(
                        "<B3-Motion>",
                        self.right_drag_tile,
                )

                self.canvas.bind(
                        "<ButtonRelease-3>",
                        self.stop_painting,
                )

                self.status_bar = tk.Label(
                        self.root,
                        text="No map loaded",
                        anchor=tk.W,
                        relief=tk.SUNKEN,
                        borderwidth=1,
                        padx=6,
                )

                self.status_bar.pack(
                        side=tk.BOTTOM,
                        fill=tk.X,
                )

                self.root.bind(
                        "<Configure>",
                        self.window_resized,
                )

        def create_map(self, event=None):
                width = simpledialog.askinteger(
                        "New Map",
                        "Width:",
                        minvalue=1,
                        maxvalue=255,
                        parent=self.root,
                )

                if width is None:
                        return

                height = simpledialog.askinteger(
                        "New Map",
                        "Height:",
                        minvalue=1,
                        maxvalue=255,
                        parent=self.root,
                )

                if height is None:
                        return

                self.width = width
                self.height = height
                self.tiles = [EMPTY] * (width * height)
                self.filename = None

                self.draw_map()
                self.update_status()

        def load_map(self, filename):
                with open(filename, "rb") as file:
                        data = file.read()

                if len(data) < 2:
                        raise ValueError("Map file is too small.")

                width = data[0]
                height = data[1]

                expected_size = 2 + width * height

                if len(data) != expected_size:
                        raise ValueError(
                                "Map file size does not match "
                                "the map dimensions."
                        )

                self.width = width
                self.height = height
                self.tiles = list(data[2:])

        def open_map(self, event=None):
                filename = filedialog.askopenfilename(
                        title="Open Map",
                        filetypes=[
                                ("Map files", "*.dat"),
                                ("All files", "*.*"),
                        ],
                )

                if not filename:
                        return

                try:
                        self.load_map(filename)
                except (OSError, ValueError) as error:
                        messagebox.showerror(
                                "Open Map",
                                str(error),
                        )
                        return

                self.filename = filename

                self.draw_map()
                self.update_status()

        def save_map(self, event=None):
                if self.width == 0 or self.height == 0:
                        return

                if self.filename is None:
                        return self.save_map_as()

                try:
                        with open(self.filename, "wb") as file:
                                file.write(
                                        bytes([
                                                self.width,
                                                self.height,
                                        ])
                                )

                                file.write(bytes(self.tiles))

                except OSError as error:
                        messagebox.showerror(
                                "Save Map",
                                str(error),
                        )

                self.update_status()

        def save_map_as(self, event=None):
                if self.width == 0 or self.height == 0:
                        return

                filename = filedialog.asksaveasfilename(
                        title="Save Map As",
                        defaultextension=".dat",
                        filetypes=[
                                ("Map files", "*.dat"),
                                ("All files", "*.*"),
                        ],
                )

                if not filename:
                        return

                self.filename = filename
                self.save_map()

        def change_grid_size(self):
                if self.width == 0 or self.height == 0:
                        return

                width = simpledialog.askinteger(
                        "Grid Size",
                        "Width:",
                        initialvalue=self.width,
                        minvalue=1,
                        maxvalue=255,
                        parent=self.root,
                )

                if width is None:
                        return

                height = simpledialog.askinteger(
                        "Grid Size",
                        "Height:",
                        initialvalue=self.height,
                        minvalue=1,
                        maxvalue=255,
                        parent=self.root,
                )

                if height is None:
                        return

                old_width = self.width
                old_height = self.height
                old_tiles = self.tiles

                self.width = width
                self.height = height
                self.tiles = [EMPTY] * (width * height)

                copy_width = min(old_width, width)
                copy_height = min(old_height, height)

                for y in range(copy_height):
                        for x in range(copy_width):
                                old_index = y * old_width + x
                                new_index = y * width + x

                                self.tiles[new_index] = old_tiles[old_index]

                self.draw_map()
                self.update_status()

        def get_grid_size(self):
                if self.width == 0 or self.height == 0:
                        return 1

                canvas_width = max(
                        self.canvas.winfo_width(),
                        1,
                )

                canvas_height = max(
                        self.canvas.winfo_height(),
                        1,
                )

                size_x = canvas_width / self.width
                size_y = canvas_height / self.height

                return max(1, int(min(size_x, size_y)))

        def draw_map(self):
                self.canvas.delete("all")

                if self.width == 0 or self.height == 0:
                        return

                grid_size = self.get_grid_size()

                for y in range(self.height):
                        for x in range(self.width):
                                index = y * self.width + x
                                tile = self.tiles[index]

                                if tile == WALL:
                                        fill = WALL_COLOR
                                else:
                                        fill = EMPTY_COLOR

                                x1 = x * grid_size
                                y1 = y * grid_size
                                x2 = x1 + grid_size
                                y2 = y1 + grid_size

                                self.canvas.create_rectangle(
                                        x1,
                                        y1,
                                        x2,
                                        y2,
                                        fill=fill,
                                        outline=GRID_COLOR,
                                )

        def get_tile_from_mouse(self, event):
                if self.width == 0 or self.height == 0:
                        return None

                grid_size = self.get_grid_size()

                x = event.x // grid_size
                y = event.y // grid_size

                if x < 0 or x >= self.width:
                        return None

                if y < 0 or y >= self.height:
                        return None

                return x, y

        def click_tile(self, event):
                tile = self.get_tile_from_mouse(event)

                if tile is None:
                        return

                x, y = tile
                index = y * self.width + x

                if event.state & 0x0001:
                        self.painting = True

                        if self.tiles[index] == WALL:
                                self.erasing = True
                                self.tiles[index] = EMPTY
                        else:
                                self.erasing = False
                                self.tiles[index] = WALL

                        self.draw_map()
                        return

                if self.tiles[index] == EMPTY:
                        self.tiles[index] = WALL
                else:
                        self.tiles[index] = EMPTY

                self.draw_map()

        def drag_tile(self, event):
                if not self.painting:
                        return

                tile = self.get_tile_from_mouse(event)

                if tile is None:
                        return

                x, y = tile
                index = y * self.width + x

                if self.erasing:
                        self.tiles[index] = EMPTY
                else:
                        self.tiles[index] = WALL

                self.draw_map()

        def right_click_tile(self, event):
                tile = self.get_tile_from_mouse(event)

                if tile is None:
                        return

                self.painting = True
                self.erasing = True

                x, y = tile
                index = y * self.width + x

                self.tiles[index] = EMPTY

                self.draw_map()

        def right_drag_tile(self, event):
                if not self.painting:
                        return

                tile = self.get_tile_from_mouse(event)

                if tile is None:
                        return

                x, y = tile
                index = y * self.width + x

                self.tiles[index] = EMPTY

                self.draw_map()

        def stop_painting(self, event=None):
                self.painting = False
                self.erasing = False

        def update_status(self):
                if self.width == 0:
                        self.status_bar.config(
                                text="No map loaded"
                        )
                        return

                filename = (
                        self.filename
                        if self.filename
                        else "Untitled"
                )

                self.status_bar.config(
                        text=(
                                f"Map: {filename}    "
                                f"Size: {self.width} x {self.height}    "
                        )
                )

        def window_resized(self, event=None):
                if hasattr(self, "canvas"):
                        self.draw_map()

        def exit_editor(self, event=None):
                self.root.destroy()


def main():
        root = tk.Tk()
        MapEditor(root)
        root.mainloop()


if __name__ == "__main__":
        main()