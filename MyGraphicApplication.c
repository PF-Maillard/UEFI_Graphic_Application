#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/GraphicsOutput.h>

EFI_STATUS EFIAPI UefiMain(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE* SystemTable)
{ 
	EFI_STATUS Status;
	EFI_GRAPHICS_OUTPUT_PROTOCOL * GraphicsProtocol = NULL;
	EFI_GRAPHICS_OUTPUT_BLT_PIXEL White = { 0xFF, 0xFF, 0xFF, 0 };
	EFI_GRAPHICS_OUTPUT_BLT_PIXEL Black = { 0x00, 0x00, 0x00, 0 };
	UINTN EventIndex;
	EFI_INPUT_KEY Key;
	INTN x = 0, y = 0;
	
	//  
	//  On recupere le protocole graphique 
	//
	Status = gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL, (VOID**)&GraphicsProtocol);
	if (EFI_ERROR(Status))
	{
		Print(L"ERROR (main): Impossible de recupérer le protocole graphique\n");
		return Status;
	}

	do 
	{
		//  
		//  On affiche les differentes formes graphiques
		//
		Status = GraphicsProtocol->Blt(GraphicsProtocol, &Black, EfiBltVideoFill, 0, 0, 0, 0, 800, 600, 0);
		if (EFI_ERROR(Status))
		{
			Print(L"ERROR (main): Impossible d'utiliser le protocole graphique\n");
			return Status;
		}

		Status = GraphicsProtocol->Blt(GraphicsProtocol, &White, EfiBltVideoFill, 0, 0, x, y, 50, 50, 0);
		if (EFI_ERROR(Status))
		{
			Print(L"ERROR (main): Impossible d'utiliser le protocole graphique\n");
			return Status;
		}

		//  
		//  On attend la pression sur une touche
		//
		Status = gBS->WaitForEvent(1, &gST->ConIn->WaitForKey, &EventIndex);
		if (EFI_ERROR(Status))
		{
			Print(L"ERROR (main): Impossible d'attendre un event\n");
			return Status;
		}

		Status = gST->ConIn->ReadKeyStroke(gST->ConIn, &Key);
		if (EFI_ERROR(Status))
		{
			Print(L"ERROR (main): Impossible de recuperer une touche\n");
			return Status;
		}

		//  
		//  Si la touche pressee correspond à une selectionee 
		//
		switch (Key.UnicodeChar)
		{
		case 'w':
			if(y > 10)
				y -= 10;
			break;
		case 's':
			if (y < 600 - 50)
				y += 10;
			break;
		case 'a':
			if (x > 10)
				x -= 10;
			break;
		case 'd':
			if (x < 800 - 50)
				x += 10;
			break;
		default:
			Print(L"Touche Impossible\n");
		}	

	} while (Key.UnicodeChar != 'y');

	return EFI_SUCCESS;
}