using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;
using TiledSharp;

namespace TiledImport
{
    internal class Program
    {

        static TmxTilesetTile GetTileFromGid(TmxMap map, int gid)
        {
            for (int i = 0; i < map.Tilesets.Count; i++)
            {
                TmxTileset testSet = map.Tilesets[i];
                if (testSet.FirstGid <= gid && (testSet.FirstGid + testSet.TileCount) > gid)
                {
                    return testSet.Tiles[gid - testSet.FirstGid];
                }
            }
            return null;
        }

        static int RoundToInt(double value)
        {
            if (value >= 0)
            {
                return (int)(Math.Round(value) + 0.00001);
            }
            else
            {
                return (int)(Math.Round(value) - 0.00001);
            }
        }

        static void Main(string[] args)
        {
            string inFile = "";
            string outFile = "";
            string arrayName = "";

            for (int i = 0; i < args.Length; i++)
            {
                string arg = args[i];
                switch (arg)
                {
                    case "-i":
                        {
                            inFile = args[++i];
                        }
                        break;
                    case "-o":
                        {
                            outFile = args[++i];
                        }
                        break;
                    case "-n":
                        {
                            arrayName = args[++i];
                        }break;
                }
            }

            if (string.IsNullOrEmpty(inFile))
            {
                Console.WriteLine("No infile set");
                return;
            }

            if (string.IsNullOrEmpty(outFile))
            {
                Console.WriteLine("No outFile set");
                return;
            }

            if (string.IsNullOrEmpty(arrayName))
            {
                Console.WriteLine("No arrayName set");
                return;
            }

            TmxMap map = new TmxMap(inFile);
            TmxObjectGroup objectGroup = null;
            foreach (TmxObjectGroup group in map.ObjectGroups)
            {
                if (group.Name == "objects")
                {
                    objectGroup = group;
                    break;
                }
            }
            if (objectGroup == null)
            {
                Console.WriteLine("Missing \"objects\" group.");
                return;
            }

            using (StreamWriter sw = new StreamWriter(outFile))
            {
                sw.WriteLine("#include \"gt_level.h\"");
                sw.WriteLine("namespace gt {");
                sw.WriteLine("constexpr const level::lvl_action "+arrayName+"[] = {");

                List<TmxObject> objectsSorted = new List<TmxObject>();

                for (int i = 0; i < objectGroup.Objects.Count; i++)
                {
                    TmxObject tmxObject = objectGroup.Objects[i];
                    objectsSorted.Add(tmxObject);
                }


                objectsSorted.Sort(delegate (TmxObject o1, TmxObject o2) { return o1.X.CompareTo(o2.X); });
                double prevX = 0;
                for(int i = 0; i < objectsSorted.Count;i++)
                {
                    TmxObject tmxObject = objectsSorted[i];
                    TmxTilesetTile tst = GetTileFromGid(map, tmxObject.Tile.Gid);

                    //Console.WriteLine($"x: {tmxObject.X} type: " + tst.Type);
                    int xDiff = (int)(tmxObject.X - prevX);
                    if(xDiff > 0)
                    {
                        sw.WriteLine("{level::LVL_CMD::WAIT_FRAMES,"+xDiff+",0,0,0},");
                    }
                    int y = RoundToInt(tmxObject.Y - (160 / 2))-16;
                    switch(tst.Type)
                    {
                        default:
                            {
                                sw.WriteLine("{level::LVL_CMD::" + tst.Type + ",136," + y + ",0,0},");
                            }
                            break;
                        case "PLAY_MUSIC":
                            {
                                string musicIndex = tmxObject.Properties["music_index"];
                                sw.WriteLine("{level::LVL_CMD::" + tst.Type + ","+musicIndex+",0,0,0},");
                            }
                            break;
                        case "SPAWN_TILESET_OBJECT":
                            {
                                string index = tmxObject.Properties["index"];
                                sw.WriteLine("{level::LVL_CMD::" + tst.Type + ","+ index + ",0,0,0},");
                            }
                            break;
                    }
                    
                    prevX = tmxObject.X;
                }


                sw.WriteLine("};");
                sw.WriteLine("}");
            }

            //Console.WriteLine("Press any key to continue ...");
            //Console.ReadKey();

        }
    }
}
