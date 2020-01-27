function GSE.TranslateString(instring, mode, cleanNewLines)
  instring = GSE.UnEscapeString(instring)
  GSE.PrintDebugMessage("Entering GSE.TranslateString with : \n" .. instring .. "\n " .. mode, GNOME)
  local output = ""
  if not GSE.isEmpty(instring) then
    if GSE.isEmpty(string.find(instring, '--', 1, true)) then
      for cmd, etc in string.gmatch(instring or '', '/(%w+)%s+([^\n]+)') do
        GSE.PrintDebugMessage("cmd : \n" .. cmd .. " etc: " .. etc, GNOME)
        output = output..GSEOptions.WOWSHORTCUTS .. "/" .. cmd .. Statics.StringReset .. " "
        if string.lower(cmd) == "use" then
          local conditionals, mods, trinketstuff = GSE.GetConditionalsFromString(etc)
          if conditionals then
            output = output .. mods .. " "
            GSE.PrintDebugMessage("GSE.TranslateSpell conditionals found ", GNOME)
          end
          GSE.PrintDebugMessage("output: " .. output .. " mods: " .. mods .. " etc: " .. etc, GNOME)

          output = output ..  GSEOptions.KEYWORD .. trinketstuff .. Statics.StringReset

        elseif Statics.CastCmds[string.lower(cmd)] then
          if not cleanNewLines then
            etc = string.match(etc, "^%s*(.-)%s*$")
          end
          if string.sub(etc, 1, 1) == "!" then
            etc = string.sub(etc, 2)
            output = output .. "!"
          end
          local foundspell, returnval = GSE.TranslateSpell(etc, mode, (cleanNewLines and cleanNewLines or false))
          if foundspell then
            output = output ..GSEOptions.KEYWORD .. returnval .. Statics.StringReset
          else
            GSE.PrintDebugMessage("Did not find : " .. etc , GNOME)
            output = output  .. etc
          end
        -- Check for cast Sequences
        elseif string.lower(cmd) == "castsequence" then
          GSE.PrintDebugMessage("attempting to split : " .. etc, GNOME)
          for x,y in ipairs(GSE.split(etc,";")) do
            for _, w in ipairs(GSE.SplitCastSequence(y,",")) do
              -- Look for conditionals at the startattack
              local conditionals, mods, uetc = GSE.GetConditionalsFromString(w)
              if conditionals then
                output = output ..GSEOptions.STANDARDFUNCS .. mods .. Statics.StringReset .. " "
              end

              if not cleanNewLines then
                w = string.match(uetc, "^%s*(.-)%s*$")
              end
              if string.sub(uetc, 1, 1) == "!" then
                uetc = string.sub(uetc, 2)
                output = output .. "!"
              end
              local foundspell, returnval = GSE.TranslateSpell(uetc, mode, (cleanNewLines and cleanNewLines or false))
              output = output ..  GSEOptions.KEYWORD .. returnval .. Statics.StringReset .. ", "
            end
            output = output .. ";"
          end
          output = string.sub(output, 1, string.len(output) -1)
          local resetleft = string.find(output, ", , ")
          if not GSE.isEmpty(resetleft) then
            output = string.sub(output, 1, resetleft -1)
          end
          if string.sub(output, string.len(output)-1) == ", " then
            output = string.sub(output, 1, string.len(output)-2)
          end

        else
          -- Pass it through
          output = output .. " " .. etc

        end
      end
      -- look for single line commands and mark them up
      for k,v in ipairs(Statics.MacroCommands) do
        output = string.gsub(output, "/"..v, GSEOptions.WOWSHORTCUTS .. "/" .. v .. Statics.StringReset)
      end
    else
      GSE.PrintDebugMessage("Detected Comment " .. string.find(instring, '--', 1, true), GNOME)
      output = output ..  GSEOptions.CONCAT .. instring .. Statics.StringReset
    end
    -- If nothing was found, pass through
    if output == "" then
      output = instring
      -- look for single line commands and mark them up
      for k,v in ipairs(Statics.MacroCommands) do
        output = string.gsub(output, "/"..v, GSEOptions.WOWSHORTCUTS .. "/" .. v .. Statics.StringReset)
      end
    end
  elseif cleanNewLines then
    output = output .. instring
  end
  GSE.PrintDebugMessage("Exiting GSE.TranslateString with : \n" .. output, GNOME)
  -- Check for random "," at the end
  if string.sub(output, string.len(output)-1) == ", " then
    output = string.sub(output, 1, string.len(output)-2)
  end
  output = string.gsub(output, ", ;", "; ")

  output = string.gsub(output, "  ", " ")
  return output
end
